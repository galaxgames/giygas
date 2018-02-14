#pragma once
#include <giygas/Framebuffer.hpp>
#include <vulkan/vulkan.h>
#include <giygas/RenderBuffer.hpp>
#include <giygas/Texture.hpp>

namespace giygas {

    class VulkanRenderer;


    class VulkanFramebuffer : public Framebuffer {

        VulkanRenderer *_renderer;
        VkFramebuffer _framebuffer;
        uint32_t _width;
        uint32_t _height;

    public:
        VulkanFramebuffer(VulkanRenderer *renderer, const FramebufferCreateParameters &params);
        VulkanFramebuffer(const VulkanFramebuffer &) = delete;
        VulkanFramebuffer &operator=(const VulkanFramebuffer &) = delete;
        VulkanFramebuffer(VulkanFramebuffer &&) = delete;
        VulkanFramebuffer &operator=(VulkanFramebuffer &&) = delete;
        ~VulkanFramebuffer() override;

        //
        // Framebuffer implementation
        //

        RendererType renderer_type() const override;
        uint32_t width() const override;
        uint32_t height() const override;

        //
        // VulkanFramebuffer implementation
        //

        VkFramebuffer handle() const;


    };

}