#include <cassert>
#include "VulkanCommandPool.hpp"
#include "VulkanRenderer.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanFramebuffer.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanVertexBuffer.hpp"
#include "VulkanIndexBuffer.hpp"

using namespace giygas;

VulkanCommandPool::VulkanCommandPool() {

}

void VulkanCommandPool::create(VulkanRenderer *renderer) {
    _renderer = renderer;

    VkCommandPoolCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    create_info.queueFamilyIndex = renderer->queue_family_indices().graphics_family;
    create_info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;

    vkCreateCommandPool(renderer->device(), &create_info, nullptr, &_handle);
}

VulkanCommandPool::~VulkanCommandPool() {
    vkDestroyCommandPool(_renderer->device(), _handle, nullptr);
}


void VulkanCommandPool::draw(const DrawInfo &info) {
    VkDevice device = _renderer->device();
    VkCommandBuffer buffer;

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


    VkCommandBufferAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = _handle;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = 1;

    vkAllocateCommandBuffers(device, &alloc_info, &buffer);

    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    begin_info.pInheritanceInfo = nullptr;

    vkBeginCommandBuffer(buffer, &begin_info);

    VkRenderPassBeginInfo pass_begin_info = {};
    pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    pass_begin_info.renderPass = pass->handle();
    pass_begin_info.framebuffer = framebuffer->handle();
    pass_begin_info.renderArea.offset = {0, 0};
    pass_begin_info.renderArea.extent.width = framebuffer->width();
    pass_begin_info.renderArea.extent.height = framebuffer->height();
    pass_begin_info.clearValueCount = 0;
    pass_begin_info.pClearValues = nullptr;

    vkCmdBeginRenderPass(buffer, &pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->handle());

    unique_ptr<VkBuffer[]> buffers(new VkBuffer[info.vertex_buffer_count]);
    unique_ptr<VkDeviceSize[]> offsets(new VkDeviceSize[info.vertex_buffer_count]);
    for (size_t i = 0; i < info.vertex_buffer_count; ++i) {
        const auto *vertex_buffer
            = reinterpret_cast<const VulkanVertexBuffer *>(info.vertex_buffers[i]);
        buffers[i] = vertex_buffer->handle();
        offsets[i] = 0;
    }

    vkCmdBindVertexBuffers(
        buffer,
        0, // first buffer
        static_cast<uint32_t>(info.vertex_buffer_count),
        buffers.get(),
        offsets.get()
    );

    vkCmdBindIndexBuffer(buffer, index_buffer->handle(), 0, index_buffer->index_type());

    vkCmdDrawIndexed(
        buffer,
        info.index_range.count,
        1,  // instance count
        info.index_range.offset,
        0,  // vertex offset
        0   // first instance
    );

    vkCmdEndRenderPass(buffer);

    vkEndCommandBuffer(buffer);

}


VkCommandPool VulkanCommandPool::handle() const {
    return _handle;
}