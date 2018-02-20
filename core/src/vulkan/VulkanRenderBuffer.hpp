//#pragma once
//#include <giygas/RenderBuffer.hpp>
//#include <vulkan/vulkan.h>
//#include "VulkanRenderTarget.hpp"
//#include "VulkanTexture.hpp"
//
//namespace giygas {
//
//    class VulkanRenderer;
//
//    class VulkanRenderBuffer final : public RenderBuffer, public VulkanRenderTarget {
//
//        VulkanRenderer *_renderer;
//        VulkanTexture _texture;
//
//    public:
//        VulkanRenderBuffer(VulkanRenderer *renderer);
//        VulkanRenderBuffer(const VulkanRenderBuffer &) = delete;
//        VulkanRenderBuffer &operator=(const VulkanRenderBuffer &) = delete;
//        VulkanRenderBuffer(VulkanRenderBuffer &&) = delete;
//        VulkanRenderBuffer &operator=(VulkanRenderBuffer &&) = delete;
//        ~VulkanRenderBuffer() override;
//
//        //
//        // RenderBuffer implementation
//        //
//
//        RendererType renderer_type() const override;
//
//        void create(
//            uint32_t width,
//            uint32_t height,
//            TextureFormat format
//        ) override;
//
//
//        //
//        // RenderTarget implementation
//        //
//
//        const void *rendertarget_impl() const override;
//
//
//        //
//        // VulkanRenderTarget implementation
//        //
//
//        VkImageView image_view() const override;
//
//    };
//
//}