#pragma once
#include <cstdint>
#include <vulkan/vulkan.h>
#include "VulkanRenderTarget.hpp"

namespace giygas {

    class VulkanSwapchain;

    class VulkanSwapchainRenderTarget final : public VulkanRenderTarget {

    public:
        const VulkanSwapchain *swapchain;
        uint32_t image_index;

        //
        // RenderTarget implementation
        //

        RendererType renderer_type() const override;
        const void *rendertarget_impl() const override;


        //
        // VulkanRenderTarget implementation
        //

        VkImageView image_view() const override;
        VkImageLayout layout() const override;
        VkFormat api_format() const override;

    };

}