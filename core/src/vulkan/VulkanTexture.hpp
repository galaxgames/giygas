#pragma once
#include <giygas/Texture.hpp>
#include <vulkan/vulkan.h>
#include <memory>

namespace giygas {
    using namespace std;

    class VulkanRenderer;

    class VulkanTexture final : public Texture {

        VulkanRenderer *_renderer;
        VkImage _image;
        VkImageView _image_view;
        unique_ptr<uint8_t[]> _data;
        size_t _size;

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
            const uint8_t *data,
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
        // VulkanTexture implementation
        //

        VkImageView image_view() const;


    };


}