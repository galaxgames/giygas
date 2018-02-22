#include <giygas/RendererType.hpp>
#include "VulkanSwapchainRenderTarget.hpp"
#include "VulkanSwapchain.hpp"

using namespace giygas;

RendererType VulkanSwapchainRenderTarget::renderer_type() const {
    return RendererType::Vulkan;
}

const void* VulkanSwapchainRenderTarget::rendertarget_impl() const {
    return static_cast<const VulkanRenderTarget *>(this);
}

VkImageView VulkanSwapchainRenderTarget::image_view() const {
    return swapchain->get_image_view(image_index);
}

VkImageLayout VulkanSwapchainRenderTarget::layout() const {
    // TODO: Get (and cache) actual layout instead of guessing!
    return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
}

VkFormat VulkanSwapchainRenderTarget::api_format() const {
    return swapchain->surface_format().format;
}
