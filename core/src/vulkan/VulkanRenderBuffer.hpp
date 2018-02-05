#pragma once
#include <giygas/RenderBuffer.hpp>
#include <vulkan/vulkan.h>

namespace giygas {

    class VulkanRenderer;

    class VulkanRenderBuffer final : public RenderBuffer {

        VulkanRenderer *_renderer;

    public:
        VulkanRenderBuffer(VulkanRenderer *renderer);
        VulkanRenderBuffer(const VulkanRenderBuffer &) = delete;
        VulkanRenderBuffer &operator=(const VulkanRenderBuffer &) = delete;
        VulkanRenderBuffer(VulkanRenderBuffer &&) = delete;
        VulkanRenderBuffer &operator=(VulkanRenderBuffer &&) = delete;
        ~VulkanRenderBuffer() override;

        //
        // RenderBuffer implementation
        //

        RendererType renderer_type() const override;

        void create_storage(
            uint32_t width,
            uint32_t height,
            TextureFormat format
        ) override;


        //
        // VulkanRenderBuffer implementation
        //

        VkImageView image_view() const;

    };

}