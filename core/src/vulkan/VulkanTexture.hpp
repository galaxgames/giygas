#pragma once
#include <giygas/Texture.hpp>
#include <vulkan/vulkan.h>
#include <memory>
#include "VulkanRenderTarget.hpp"

namespace giygas {
    using namespace std;

    class VulkanRenderer;

    class VulkanTexture final : public Texture, public VulkanRenderTarget {

        VulkanRenderer *_renderer;
        VkImage _image;
        VkImageView _image_view;
        VkDeviceMemory _image_memory;
        unique_ptr<uint8_t[]> _data;
        size_t _size;
        uint32_t _width;
        uint32_t _height;
        TextureFormat _format;
        VkFormat _api_format;
        VkImageLayout _layout;

        void create_image(
            uint32_t width,
            uint32_t height,
            VkFormat format,
            VkImageTiling tiling,
            VkImageUsageFlags usage_flags,
            VkMemoryPropertyFlags memory_properties,
            VkImage &image,
            VkDeviceMemory &image_memory,
            VkImageLayout initial_layout
        ) const;

        void transition_image_layout(
            VkImage image,
            TextureFormat format,
            VkImageLayout old_layout,
            VkImageLayout new_layout
        ) const;

        void copy_buffer_to_image(
            VkBuffer buffer,
            VkImage image,
            uint32_t width,
            uint32_t height
        );

        VkCommandBuffer begin_command_buffer() const;
        void end_command_buffer(VkCommandBuffer buffer) const;

        static VkImageAspectFlags image_aspects_from_format(TextureFormat format);

    public:
        explicit VulkanTexture(VulkanRenderer *renderer);
        VulkanTexture(const VulkanTexture &) = delete;
        VulkanTexture &operator=(const VulkanTexture &) = delete;
        VulkanTexture(VulkanTexture &&) = delete;
        VulkanTexture &operator=(VulkanTexture &&) = delete;
        ~VulkanTexture() override;

        //
        // Texture implementation
        //

        RendererType renderer_type() const override;

        void create(
            unique_ptr<uint8_t[]> &&data,
            size_t size,
            uint32_t width,
            uint32_t height,
            TextureFormat input_format,
            TextureFormat desired_format,
            TextureUsageFlags flags
        ) override;

        TextureFormat format() const override;
        const void *texture_impl() const override;


        //
        // RenderTarget implementation
        //

        const void *rendertarget_impl() const override;


        //
        // VulkanRenderTarget implementation
        //

        VkImageView image_view() const override;
        VkImageLayout layout() const override;
        VkFormat api_format() const override;

        //
        // VulkanTexture implementation
        //


//        void create(
//            unique_ptr<uint8_t[]> &&data,
//            size_t size,
//            uint32_t width,
//            uint32_t height,
//            TextureFormat format,
//            VkImageUsageFlags usage_flags,
//            VkImageLayout final_layout
//        );

    };


}