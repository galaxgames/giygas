#include "VulkanCommandPool.hpp"
#include "VulkanRenderer.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanVertexBuffer.hpp"
#include "VulkanIndexBuffer.hpp"
#include "VulkanCommandBuffer.hpp"

using namespace giygas;

VulkanCommandPool::VulkanCommandPool(VulkanRenderer *renderer) {
    _renderer = renderer;
}

VulkanCommandPool::~VulkanCommandPool() {
    destroy();
}

RendererType VulkanCommandPool::renderer_type() const {
    return RendererType::Vulkan;
}

void VulkanCommandPool::create() {
    uint32_t image_count = _renderer->swapchain_image_count();
    assert(image_count > 0);

    VkCommandPool *pools = new VkCommandPool[image_count];

    for (uint32_t i = 0; i < image_count; ++i) {
        pools[i] = make_pool();
    }

    _handles = unique_ptr<VkCommandPool[]>(pools);
}

void VulkanCommandPool::destroy() {
    if (_renderer == nullptr || _handles == nullptr) {
        return;
    }

    VkDevice device = _renderer->device();

    for (uint32_t i = 0, ilen = _renderer->swapchain_image_count(); i < ilen; ++i) {
        vkDestroyCommandPool(device, _handles[i], nullptr);
    }

    _handles = nullptr;
}

void VulkanCommandPool::reset_buffers() {
    vkResetCommandPool(_renderer->device(), _handles[_renderer->next_swapchain_image_index()], 0);
}

bool VulkanCommandPool::is_valid() const {
    return _handles != nullptr;
}

VkCommandPool VulkanCommandPool::get_handle(uint32_t index) const {
    assert(_handles != nullptr);
    assert(index < _renderer->swapchain_image_count());
    return _handles[index];
}

VkCommandPool VulkanCommandPool::make_pool() const {
    VkCommandPool handle;
    VkCommandPoolCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    create_info.queueFamilyIndex = _renderer->queue_family_indices().graphics_family;
    create_info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;

    vkCreateCommandPool(_renderer->device(), &create_info, nullptr, &handle);
    return handle;
}
