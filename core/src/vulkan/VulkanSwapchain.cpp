#include <cstdint>
#include <cassert>
#include "VulkanSwapchain.hpp"
#include "VulkanRenderer.hpp"

using namespace giygas;

VulkanSwapchain::VulkanSwapchain() {
    _renderer = nullptr;
    _image_count = 0;
    _image_view_count = 0;
    _swapchain = VK_NULL_HANDLE;
}

VulkanSwapchain::~VulkanSwapchain() {
    destroy();
}

uint32_t VulkanSwapchain::width() const {
    return _extent.width;
}

uint32_t VulkanSwapchain::height() const {
    return _extent.height;
}

bool VulkanSwapchain::create(
    VulkanRenderer *renderer,
    VkSurfaceKHR surface,
    VkSurfaceFormatKHR surface_format,
    VkPresentModeKHR present_mode,
    VkExtent2D extent,
    const SwapchainInfo &info
) {
    _renderer = renderer;
    _format = surface_format;
    _extent = extent;

    VkDevice device = renderer->device();

    if (create_swapchain(
        device,
        surface,
        surface_format,
        present_mode,
        extent,
        info,
        _renderer->queue_family_indices(),
        _swapchain
    ) != VK_SUCCESS) {
        return false;
    }

    _image_count = get_swapchain_images(device, _swapchain, _images);
    _image_view_count = _image_count;
    bool success = (
        create_image_views(
            _image_count,
            _images.get(),
            _format.format,
            device,
            _image_views
        ) != VK_SUCCESS
    );

    _render_targets = unique_ptr<VulkanSwapchainRenderTarget[]>(
        new VulkanSwapchainRenderTarget[_image_count]
    );
    for (uint32_t i = 0; i < _image_count; ++i) {
        VulkanSwapchainRenderTarget &target = _render_targets[i];
        target.swapchain = this;
        target.image_index = i;
    }

    return success;
}

void VulkanSwapchain::destroy() {
    if (_renderer == nullptr) {
        return;
    }
    VkDevice device = _renderer->device();
    for (unsigned int i = 0; i < _image_view_count; ++i) {
        vkDestroyImageView(device, _image_views[i], nullptr);
    }
    _image_view_count = 0;
    _image_views = nullptr;
    vkDestroySwapchainKHR(device, _swapchain, nullptr);
    _swapchain = VK_NULL_HANDLE;
    _renderer = nullptr;
}

VkSwapchainKHR VulkanSwapchain::handle() const {
    return _swapchain;
}

uint32_t VulkanSwapchain::image_count() const {
    return _image_count;
}

const VkSurfaceFormatKHR& VulkanSwapchain::surface_format() const {
    return _format;
}

VkImageView VulkanSwapchain::get_image_view(uint32_t index) const {
    assert(index < _image_count);
    return _image_views[index];
}

const VulkanSwapchainRenderTarget* VulkanSwapchain::get_render_target(uint32_t index) const {
    assert(index < _image_count);
    return &_render_targets[index];
}


VkResult VulkanSwapchain::create_swapchain(
    VkDevice device,
    VkSurfaceKHR surface,
    VkSurfaceFormatKHR surface_format,
    VkPresentModeKHR present_mode,
    VkExtent2D extent,
    const SwapchainInfo &info,
    QueueFamilyIndices indices,
    VkSwapchainKHR &swapchain
) {
    unsigned int image_count = info.capabilities.minImageCount + 1;
    if (
        info.capabilities.maxImageCount > 0 &&
        image_count > info.capabilities.maxImageCount
        ) {
        image_count = info.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = surface;
    create_info.minImageCount = image_count;
    create_info.imageFormat = surface_format.format;
    create_info.imageColorSpace = surface_format.colorSpace;
    create_info.imageExtent = extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (indices.graphics_family != indices.present_family) {
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = reinterpret_cast<unsigned int *>(&indices);
    }
    else {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        create_info.queueFamilyIndexCount = 0;
        create_info.pQueueFamilyIndices = nullptr;
    }

    create_info.preTransform = info.capabilities.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = present_mode;
    create_info.clipped = VK_TRUE;
    create_info.oldSwapchain = VK_NULL_HANDLE;

    return vkCreateSwapchainKHR(device, &create_info, nullptr, &swapchain);
}

unsigned int VulkanSwapchain::get_swapchain_images(
    VkDevice device,
    VkSwapchainKHR swapchain,
    unique_ptr<VkImage[]> &dest
) {
    unsigned int count;
    vkGetSwapchainImagesKHR(device, swapchain, &count, nullptr);
    dest = unique_ptr<VkImage[]>(new VkImage[count]);
    vkGetSwapchainImagesKHR(device, swapchain, &count, dest.get());
    return count;
}

VkResult VulkanSwapchain::create_image_views(
    unsigned int count,
    const VkImage *images,
    VkFormat format,
    VkDevice device,
    unique_ptr<VkImageView[]> &views
) {
    views = unique_ptr<VkImageView[]>(new VkImageView[count]);
    VkImageViewCreateInfo create_info;
    VkResult create_result;

    create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    create_info.format = format;
    create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    create_info.subresourceRange.baseMipLevel = 0;
    create_info.subresourceRange.levelCount = 1;
    create_info.subresourceRange.baseArrayLayer = 0;
    create_info.subresourceRange.layerCount = 1;

    for (unsigned int i = 0; i < count; ++i){
        views[i] = nullptr;
    }
    for (unsigned int i = 0; i < count; ++i) {
        create_info.image = images[i];

        create_result = vkCreateImageView(
            device,
            &create_info,
            nullptr,
            &views[i]
        );
        if (create_result != VK_SUCCESS) {
            return create_result;
        }
    }
    return VK_SUCCESS;
}
