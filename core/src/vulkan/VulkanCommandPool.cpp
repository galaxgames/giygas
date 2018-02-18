#include "VulkanCommandPool.hpp"
#include "VulkanRenderer.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanVertexBuffer.hpp"
#include "VulkanIndexBuffer.hpp"
#include "VulkanCommandBuffer.hpp"

using namespace giygas;

VulkanCommandPool::VulkanCommandPool() {
    _renderer = nullptr;
    _handle = VK_NULL_HANDLE;
}

VulkanCommandPool::~VulkanCommandPool() {
    destroy();
}

void VulkanCommandPool::create(VulkanRenderer *renderer) {
    _renderer = renderer;

    VkCommandPoolCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    create_info.queueFamilyIndex = renderer->queue_family_indices().graphics_family;
    create_info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;

    vkCreateCommandPool(renderer->device(), &create_info, nullptr, &_handle);
}

void VulkanCommandPool::destroy() {
    if (_renderer == nullptr) {
        return;
    }
    vkDestroyCommandPool(_renderer->device(), _handle, nullptr);
    _handle = VK_NULL_HANDLE;
    _renderer = nullptr;
}

CommandBuffer VulkanCommandPool::take_static_buffer() {
    auto *buffer = new VulkanCommandBuffer(_renderer, this, true);
    buffer->create();
    return CommandBuffer(buffer);
}

CommandBuffer VulkanCommandPool::take_dynamic_buffer() {
    return CommandBuffer(nullptr); // TODO
}

void VulkanCommandPool::return_buffer(VulkanCommandBuffer *buffer) {
    if (buffer->is_static()) {
        delete buffer;
    }
    else {
        // TODO
    }
}

VkCommandPool VulkanCommandPool::handle() const {
    return _handle;
}