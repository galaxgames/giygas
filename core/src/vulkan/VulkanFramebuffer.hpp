#pragma once
#include <giygas/Framebuffer.hpp>
#include <vulkan/vulkan.h>
#include <giygas/Texture.hpp>
#include "VulkanRenderTarget.hpp"

namespace giygas {

    class VulkanRenderer;


    class VulkanFramebuffer : public Framebuffer {

        VulkanRenderer *_renderer;
        VkFramebuffer _handle;
        uint32_t _width;
        uint32_t _height;
        size_t _attachment_count;
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

        VkFramebuffer handle() const;
        //const AttachmentPurpose *attachment_purposes() const;


    };

}