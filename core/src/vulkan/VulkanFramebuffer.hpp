#pragma once
#include <giygas/Framebuffer.hpp>
#include <vulkan/vulkan.h>
#include <giygas/Texture.hpp>
#include "VulkanRenderTarget.hpp"

namespace giygas {

    class VulkanRenderer;

    class VulkanFramebuffer : public Framebuffer {

        VulkanRenderer *_renderer = nullptr;
        bool _is_for_swapchain = false;
        uint32_t _handle_count = 0;
        unique_ptr<VkFramebuffer[]> _handles;
        uint32_t _width = 0;
        uint32_t _height = 0;
        size_t _attachment_count = 0;
        unique_ptr<AttachmentPurpose[]> _purposes;

    public:
        explicit VulkanFramebuffer(VulkanRenderer *renderer);
        VulkanFramebuffer(const VulkanFramebuffer &) = delete;
        VulkanFramebuffer &operator=(const VulkanFramebuffer &) = delete;
        VulkanFramebuffer(VulkanFramebuffer &&) = delete;
        VulkanFramebuffer &operator=(VulkanFramebuffer &&) = delete;
        ~VulkanFramebuffer() override;

        //
        // Framebuffer implementation
        //

        RendererType renderer_type() const override;
        void create(const FramebufferCreateParameters &params) override;
        bool is_valid() const override;
        uint32_t width() const override;
        uint32_t height() const override;
        size_t attachment_count() const override;
        const AttachmentPurpose *attachment_purposes() const override;

        //
        // VulkanFramebuffer implementation
        //
        bool is_for_swapchain() const;
        VkFramebuffer get_handle(uint32_t index) const;

    };

}
