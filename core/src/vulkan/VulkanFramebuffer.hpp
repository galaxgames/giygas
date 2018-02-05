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

    public:
        VulkanFramebuffer(VulkanRenderer *renderer, const FramebufferCreateParameters &params);
        VulkanFramebuffer(const VulkanFramebuffer &) = delete;
        VulkanFramebuffer &operator=(const VulkanFramebuffer &) = delete;
        VulkanFramebuffer(VulkanFramebuffer &&) = delete;
        VulkanFramebuffer &operator=(VulkanFramebuffer &&) = delete;
        ~VulkanFramebuffer() override;


        //
        // VulkanFramebuffer implementation
        //

        VkFramebuffer handle() const;


    };

}