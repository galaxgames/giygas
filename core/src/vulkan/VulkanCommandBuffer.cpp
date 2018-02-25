#include <cassert>
#include "VulkanCommandBuffer.hpp"
#include "VulkanCommandPool.hpp"
#include "VulkanRenderer.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanFramebuffer.hpp"
#include "VulkanIndexBuffer.hpp"
#include "VulkanVertexBuffer.hpp"
#include "VulkanDescriptorSet.hpp"

using namespace giygas;

VulkanCommandBuffer::VulkanCommandBuffer(
    VulkanRenderer *renderer,
    VulkanCommandPool *pool
) {
    _renderer = renderer;
    _pool = pool;
    _handle = VK_NULL_HANDLE;
}

VulkanCommandBuffer::~VulkanCommandBuffer() {
    vkFreeCommandBuffers(_renderer->device(), _pool->handle(), 1, &_handle);
}

RendererType VulkanCommandBuffer::renderer_type() const {
    return RendererType::Vulkan;
}

void VulkanCommandBuffer::create() {
    VkCommandBufferAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = _pool->handle();
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = 1;

    vkAllocateCommandBuffers(_renderer->device(), &alloc_info, &_handle);
}

void VulkanCommandBuffer::record(const DrawInfo &info) {
    VkDevice device = _renderer->device();

#ifndef NDEBUG
    assert(info.pipeline != nullptr);
    assert(info.pipeline->renderer_type() == RendererType::Vulkan);
    //assert(info.pass != nullptr);
    //assert(info.pass->renderer_type() == RendererType::Vulkan);
    assert(info.framebuffer != nullptr);
    assert(info.framebuffer->renderer_type() == RendererType::Vulkan);
    for (size_t i = 0; i < info.vertex_buffer_count; ++i) {
        const VertexBuffer *vertex_buffer = info.vertex_buffers[i];
        assert(vertex_buffer->renderer_type() == RendererType::Vulkan);
    }
    assert(info.index_buffer != nullptr);
    assert(info.index_buffer->renderer_type() == RendererType::Vulkan);

    // Validate descriptor set
    if (info.pipeline->descriptor_set_count() > 0) {
        assert(info.descriptor_set != nullptr);
        assert(info.descriptor_set->renderer_type() == RendererType::Vulkan);
        assert(info.pipeline->is_descriptor_set_compatible(info.descriptor_set));
    }
    else {
        assert(info.descriptor_set == nullptr);
    }

#endif

    const auto *pipeline = reinterpret_cast<const VulkanPipeline *>(info.pipeline);
    //const auto *pass = reinterpret_cast<const VulkanRenderPass *>(info.pass);
    const auto *framebuffer = reinterpret_cast<const VulkanFramebuffer *>(info.framebuffer);
    const auto *index_buffer
        = reinterpret_cast<const VulkanGenericIndexBuffer *>(info.index_buffer->cast_to_specific());
    const auto *descriptor_set = reinterpret_cast<const VulkanDescriptorSet *>(info.descriptor_set);

    size_t attachment_count = framebuffer->attachment_count();

    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = 0; //VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;  // TODO
    begin_info.pInheritanceInfo = nullptr;

    vkBeginCommandBuffer(_handle, &begin_info);

    // TODO: Reduce frequency of allocation.
    unique_ptr<VkClearValue[]> clear_values(new VkClearValue[attachment_count]);
    const AttachmentPurpose *purposes = framebuffer->purposes();
    for (size_t i = 0; i < attachment_count; ++i) {
        const ClearValue &clear_value = info.clear_values[i];
        VkClearValue &api_clear_val = clear_values[i];
        AttachmentPurpose purpose = clear_value.purpose;
        assert(purposes[i] == purpose);
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
    pass_begin_info.renderPass = framebuffer->renderpass();
    pass_begin_info.framebuffer = framebuffer->handle();
    pass_begin_info.renderArea.offset = {0, 0};
    pass_begin_info.renderArea.extent.width = framebuffer->width();
    pass_begin_info.renderArea.extent.height = framebuffer->height();
    pass_begin_info.clearValueCount = static_cast<uint32_t>(attachment_count);
    pass_begin_info.pClearValues = clear_values.get();

    vkCmdBeginRenderPass(_handle, &pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

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

    vkCmdEndRenderPass(_handle);

    vkEndCommandBuffer(_handle);

}

VkCommandBuffer VulkanCommandBuffer::handle() const {
    return _handle;
}

