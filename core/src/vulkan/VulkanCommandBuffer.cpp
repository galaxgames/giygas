#include <cassert>
#include "VulkanCommandBuffer.hpp"
#include "VulkanCommandPool.hpp"
#include "VulkanRenderer.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanIndexBuffer.hpp"
#include "VulkanVertexBuffer.hpp"
#include "VulkanDescriptorSet.hpp"
#include "VulkanRenderPass.hpp"
#include <giygas/validation/command_buffer_validation.hpp>

using namespace giygas;
using namespace giygas::validation;

VulkanCommandBuffer::VulkanCommandBuffer(VulkanRenderer *renderer) {
    _renderer = renderer;
    _pool = nullptr;
    _handle = VK_NULL_HANDLE;
}

VulkanCommandBuffer::~VulkanCommandBuffer() {
    vkFreeCommandBuffers(_renderer->device(), _pool->handle(), 1, &_handle);
}

RendererType VulkanCommandBuffer::renderer_type() const {
    return RendererType::Vulkan;
}

void VulkanCommandBuffer::create(CommandPool *pool) {
    assert(validate_command_buffer_create(this, pool));

    _pool = reinterpret_cast<const VulkanCommandPool *>(pool);

    VkCommandBufferAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = _pool->handle();
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = 1;

    vkAllocateCommandBuffers(_renderer->device(), &alloc_info, &_handle);
}

void VulkanCommandBuffer::record_pass(const SingleBufferPassInfo &info) {
    assert(validate_command_buffer_record_pass(this, info));

    VkDevice device = _renderer->device();

    const auto *pass_impl = reinterpret_cast<const VulkanRenderPass *>(info.pass_info.pass);
    const auto *framebuffer_impl = reinterpret_cast<const VulkanFramebuffer *>(info.pass_info.framebuffer);


    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = 0; //VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;  // TODO
    begin_info.pInheritanceInfo = nullptr;

    vkBeginCommandBuffer(_handle, &begin_info);

    // TODO: Assert framebuffer and renderpass are compatible!

    size_t attachment_count = framebuffer_impl->attachment_count();

    // TODO: Reduce frequency of allocation.
    unique_ptr<VkClearValue[]> clear_values(new VkClearValue[attachment_count]);
    const AttachmentPurpose *purposes = pass_impl->attachment_purposes();
    for (size_t i = 0; i < attachment_count; ++i) {
        const ClearValue &clear_value = info.pass_info.clear_values[i];
        VkClearValue &api_clear_val = clear_values[i];
        AttachmentPurpose purpose = clear_value.purpose;
        if (purpose == AttachmentPurpose::Color) {
            api_clear_val = {
                clear_value.color_value.x,
                clear_value.color_value.y,
                clear_value.color_value.z,
                clear_value.color_value.w
            };
        }
        else {
            api_clear_val.depthStencil.depth = clear_value.depth_stencil.depth;
            api_clear_val.depthStencil.stencil = clear_value.depth_stencil.stencil;
        }
    }

    VkRenderPassBeginInfo pass_begin_info = {};
    pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    pass_begin_info.renderPass = pass_impl->handle();
    pass_begin_info.framebuffer = framebuffer_impl->handle();
    pass_begin_info.renderArea.offset = {0, 0};
    pass_begin_info.renderArea.extent.width = framebuffer_impl->width();
    pass_begin_info.renderArea.extent.height = framebuffer_impl->height();
    pass_begin_info.clearValueCount = static_cast<uint32_t>(attachment_count);
    pass_begin_info.pClearValues = clear_values.get();

    vkCmdBeginRenderPass(_handle, &pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

    for (size_t i = 0; i < info.draw_count; ++i) {
        record_draw(info.draws[i]);
    }

    vkCmdEndRenderPass(_handle);

    vkEndCommandBuffer(_handle);

}

void VulkanCommandBuffer::record_draw(const DrawInfo &info) const {
    const auto *pipeline = reinterpret_cast<const VulkanPipeline *>(info.pipeline);
    const auto *index_buffer
        = reinterpret_cast<const VulkanGenericIndexBuffer *>(info.index_buffer->cast_to_specific());
    const auto *descriptor_set = reinterpret_cast<const VulkanDescriptorSet *>(info.descriptor_set);


    vkCmdBindPipeline(_handle, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->handle());

    // TODO: Reduce frequency of allocation.
    unique_ptr<VkBuffer[]> buffers(new VkBuffer[info.vertex_buffer_count]);
    unique_ptr<VkDeviceSize[]> offsets(new VkDeviceSize[info.vertex_buffer_count]);
    for (size_t i = 0; i < info.vertex_buffer_count; ++i) {
        const auto *vertex_buffer
            = reinterpret_cast<const VulkanVertexBuffer *>(info.vertex_buffers[i]);
        buffers[i] = vertex_buffer->handle();
        offsets[i] = 0;
    }

    vkCmdBindVertexBuffers(
        _handle,
        0, // first buffer
        static_cast<uint32_t>(info.vertex_buffer_count),
        buffers.get(),
        offsets.get()
    );

    vkCmdBindIndexBuffer(_handle, index_buffer->handle(), 0, index_buffer->index_type());

    if (info.push_constants_size > 0) {
        vkCmdPushConstants(
            _handle,
            pipeline->layout_handle(),
            VK_SHADER_STAGE_ALL,
            static_cast<uint32_t>(info.push_constants_offset * sizeof(uint8_t)),
            static_cast<uint32_t>(info.push_constants_size * sizeof(uint8_t)),
            info.push_constants
        );
    }

    if (info.descriptor_set != nullptr) {
        VkDescriptorSet descriptor_set_handle = descriptor_set->handle();
        vkCmdBindDescriptorSets(
            _handle,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            pipeline->layout_handle(),
            0,
            1,
            &descriptor_set_handle,
            0,
            nullptr
        );
    }

    vkCmdDrawIndexed(
        _handle,
        info.index_range.count,
        1,  // instance count
        info.index_range.offset,
        0,  // vertex offset
        0   // first instance
    );

}

bool VulkanCommandBuffer::is_valid() const {
    return _handle != VK_NULL_HANDLE;
}

VkCommandBuffer VulkanCommandBuffer::handle() const {
    return _handle;
}

