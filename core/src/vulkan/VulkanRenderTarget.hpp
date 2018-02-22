#pragma once
#include <giygas/RenderTarget.hpp>

namespace giygas {

    class VulkanRenderTarget : public RenderTarget {
    public:
        virtual VkImageView image_view() const = 0;
        virtual VkImageLayout layout() const = 0;
        virtual VkFormat api_format() const = 0;
    };

}