#include "VulkanRenderBuffer.hpp"

using namespace giygas;

VulkanRenderBuffer::VulkanRenderBuffer(VulkanRenderer *renderer) {
    _renderer = renderer;
}

VulkanRenderBuffer::~VulkanRenderBuffer() {

}

RendererType VulkanRenderBuffer::renderer_type() const {
    return RendererType::Vulkan;
}

void VulkanRenderBuffer::create_storage(uint32_t width, uint32_t height, TextureFormat format) {


}

const void* VulkanRenderBuffer::impl() const {
    return static_cast<const VulkanRenderTarget *>(this);
}

VkImageView VulkanRenderBuffer::image_view() const {
    return VK_NULL_HANDLE;
}
