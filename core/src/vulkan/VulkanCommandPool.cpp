#include "VulkanCommandPool.hpp"
#include "VulkanRenderer.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanVertexBuffer.hpp"
#include "VulkanIndexBuffer.hpp"
#include "VulkanCommandBuffer.hpp"

using namespace giygas;

VulkanCommandPool::VulkanCommandPool(VulkanRenderer *renderer) {
    _renderer = renderer;
    _handle = VK_NULL_HANDLE;
}

VulkanCommandPool::~VulkanCommandPool() {
    destroy();
}

RendererType VulkanCommandPool::renderer_type() const {
    return RendererType::Vulkan;
}

void VulkanCommandPool::create() {
    VkCommandPoolCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    create_info.queueFamilyIndex = _renderer->queue_family_indices().graphics_family;
    create_info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;

    vkCreateCommandPool(_renderer->device(), &create_info, nullptr, &_handle);
}

void VulkanCommandPool::destroy() {
    if (_renderer == nullptr) {
        return;
    }
    vkDestroyCommandPool(_renderer->device(), _handle, nullptr);
    _handle = VK_NULL_HANDLE;
    _renderer = nullptr;
}

void VulkanCommandPool::reset_buffers() {
    vkResetCommandPool(_renderer->device(), _handle, 0);
}

bool VulkanCommandPool::is_valid() const {
    return _handle != VK_NULL_HANDLE;
}

VkCommandPool VulkanCommandPool::handle() const {
    return _handle;
}