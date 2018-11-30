#pragma once
#include <cstdint>
#include <vulkan/vulkan.h>
#include "VulkanRenderTarget.hpp"

namespace giygas {

    class VulkanSwapchain;

    class VulkanSwapchainRenderTarget final : public VulkanRenderTarget {

        const VulkanSwapchain *_swapchain = nullptr;

    public:

        VulkanSwapchainRenderTarget(const VulkanSwapchain *swapchain);

        //
        // RenderTarget implementation
        //

        RendererType renderer_type() const override;
        const void *rendertarget_impl() const override;
        uint32_t width() const override;
        uint32_t height() const override;


        //
        // VulkanRenderTarget implementation
        //

        VkImageView image_view(uint32_t index) const override;
        VkImageLayout layout() const override;
        VkFormat api_format() const override;
        bool is_swapchain() const override;

    };

}
