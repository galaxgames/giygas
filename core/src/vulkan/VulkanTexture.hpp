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

        static VkFormat translate_format(TextureFormat format);

        void create_image(
            uint32_t width,
            uint32_t height,
            VkFormat format,
            VkImageTiling tiling,
            VkImageUsageFlags usage_flags,
            VkMemoryPropertyFlags memory_properties,
            VkImage &image,
            VkDeviceMemory &image_memory
        ) const;

        void transition_image_layout(
            VkImage image,
            VkFormat format,
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

        void set_data(
            unique_ptr<uint8_t[]> &&data,
            size_t size,
            uint32_t width,
            uint32_t height,
            TextureFormat format
        ) override;

        void create_storage(
            uint32_t width,
            uint32_t height,
            TextureFormat format
        ) override;


        //
        // RenderTarget implementation
        //

        const void *impl() const override;

        //
        // VulkanRenderTarget implementation
        //

        VkImageView image_view() const override;

    };


}