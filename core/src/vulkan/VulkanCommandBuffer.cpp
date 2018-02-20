#include <cassert>
#include "VulkanCommandBuffer.hpp"
#include "VulkanCommandPool.hpp"
#include "VulkanRenderer.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanFramebuffer.hpp"
#include "VulkanIndexBuffer.hpp"
#include "VulkanVertexBuffer.hpp"

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
    assert(info.pass != nullptr);
    assert(info.pass->renderer_type() == RendererType::Vulkan);
    assert(info.framebuffer != nullptr);
    assert(info.framebuffer->renderer_type() == RendererType::Vulkan);
    for (size_t i = 0; i < info.vertex_buffer_count; ++i) {
        const VertexBuffer *vertex_buffer = info.vertex_buffers[i];
        assert(vertex_buffer->renderer_type() == RendererType::Vulkan);
    }
    assert(info.index_buffer != nullptr);
    assert(info.index_buffer->renderer_type() == RendererType::Vulkan);
#endif

    const auto *pipeline = reinterpret_cast<const VulkanPipeline *>(info.pipeline);
    const auto *pass = reinterpret_cast<const VulkanRenderPass *>(info.pass);
    const auto *framebuffer = reinterpret_cast<const VulkanFramebuffer *>(info.framebuffer);
    const auto *index_buffer
        = reinterpret_cast<const VulkanGenericIndexBuffer *>(info.index_buffer->cast_to_specific());

    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = 0; //VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;  // TODO
    begin_info.pInheritanceInfo = nullptr;

    vkBeginCommandBuffer(_handle, &begin_info);

    VkRenderPassBeginInfo pass_begin_info = {};
    pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    pass_begin_info.renderPass = pass->handle();
    pass_begin_info.framebuffer = framebuffer->handle();
    pass_begin_info.renderArea.offset = {0, 0};
    pass_begin_info.renderArea.extent.width = framebuffer->width();
    pass_begin_info.renderArea.extent.height = framebuffer->height();
    VkClearValue clear_color = { 1.0f, 0.1f, 0.1f, 0.1f };
    pass_begin_info.clearValueCount = 1;
    pass_begin_info.pClearValues = &clear_color;

    vkCmdBeginRenderPass(_handle, &pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(_handle, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->handle());

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

