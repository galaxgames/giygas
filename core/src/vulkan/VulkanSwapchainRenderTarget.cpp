#include <giygas/RendererType.hpp>
#include "VulkanSwapchainRenderTarget.hpp"
#include "VulkanSwapchain.hpp"

using namespace giygas;

RendererType VulkanSwapchainRenderTarget::renderer_type() const {
    return RendererType::Vulkan;
}

const void* VulkanSwapchainRenderTarget::impl() const {
    return static_cast<const VulkanRenderTarget *>(this);
}

VkImageView VulkanSwapchainRenderTarget::image_view() const {
    return swapchain->get_image_view(image_index);
}