#pragma once
#include <giygas/RenderTarget.hpp>
#include <vulkan/vulkan.h>

namespace giygas {

    class VulkanRenderTarget : public RenderTarget {
    public:
        virtual VkImageView image_view(uint32_t index) const = 0;
        virtual VkImageLayout layout() const = 0;
        virtual VkFormat api_format() const = 0;
        virtual bool is_swapchain() const = 0;
    };

}
