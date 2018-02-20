//#include <giygas/giygas.hpp>
//#include "VulkanRenderBuffer.hpp"
//
//using namespace giygas;
//
//VulkanRenderBuffer::VulkanRenderBuffer(VulkanRenderer *renderer)
//    : _texture(renderer)
//{
//    _renderer = renderer;
//}
//
//VulkanRenderBuffer::~VulkanRenderBuffer() {
//}
//
//RendererType VulkanRenderBuffer::renderer_type() const {
//    return RendererType::Vulkan;
//}
//
//void VulkanRenderBuffer::create(uint32_t width, uint32_t height, TextureFormat format) {
//    VkImageUsageFlags usage_flags;
//    VkImageLayout layout;
//    switch (attachment_purpose_from_texture_format(format)) {
//        case AttachmentPurpose::Depth:
//        case AttachmentPurpose::Stencil:
//            usage_flags = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
//            layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
//            break;
//        case AttachmentPurpose::Color:
//            usage_flags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
//            layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//            break;
//    }
//
//    _texture.create(nullptr, 0, width, height, format, usage_flags, layout);
//}
//
//const void* VulkanRenderBuffer::rendertarget_impl() const {
//    return static_cast<const VulkanRenderTarget *>(this);
//}
//
//VkImageView VulkanRenderBuffer::image_view() const {
//    return _texture.image_view();
//}
