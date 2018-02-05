#include "VulkanTexture.hpp"

using namespace giygas;

VulkanTexture::VulkanTexture(VulkanRenderer *renderer) {
    _renderer = renderer;
    _image = VK_NULL_HANDLE;
    _image_view = VK_NULL_HANDLE;
}

VulkanTexture::~VulkanTexture() {

}

RendererType VulkanTexture::renderer_type() const {
    return RendererType::Vulkan;
}

void VulkanTexture::set_data(
    const uint8_t *data,
    size_t size,
    uint32_t width,
    uint32_t height,
    TextureFormat format
) {



}

void VulkanTexture::create_storage(
    uint32_t width,
    uint32_t height,
    TextureFormat format
) {

}


VkImageView VulkanTexture::image_view() const {
    return _image_view;
}



