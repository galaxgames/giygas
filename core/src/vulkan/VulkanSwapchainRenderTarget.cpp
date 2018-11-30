#include <giygas/RendererType.hpp>
#include "VulkanSwapchainRenderTarget.hpp"
#include "VulkanSwapchain.hpp"

using namespace giygas;

VulkanSwapchainRenderTarget::VulkanSwapchainRenderTarget(const VulkanSwapchain *swapchain) {
    _swapchain = swapchain;
}

RendererType VulkanSwapchainRenderTarget::renderer_type() const {
    return RendererType::Vulkan;
}

const void* VulkanSwapchainRenderTarget::rendertarget_impl() const {
    return static_cast<const VulkanRenderTarget *>(this);
}

uint32_t VulkanSwapchainRenderTarget::width() const {
    return _swapchain->width();
}

uint32_t VulkanSwapchainRenderTarget::height() const {
    return _swapchain->height();
}

VkImageView VulkanSwapchainRenderTarget::image_view(uint32_t index) const {
    return _swapchain->get_image_view(index);
}

VkImageLayout VulkanSwapchainRenderTarget::layout() const {
    // TODO: Get (and cache) actual layout instead of guessing!
    return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
}

VkFormat VulkanSwapchainRenderTarget::api_format() const {
    return _swapchain->surface_format().format;
}

bool VulkanSwapchainRenderTarget::is_swapchain() const {
    return true;
}
