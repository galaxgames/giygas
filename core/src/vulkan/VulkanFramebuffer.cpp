#include <cassert>
#include "VulkanFramebuffer.hpp"
#include "VulkanRenderer.hpp"
#include "VulkanTexture.hpp"
#include "VulkanRenderBuffer.hpp"
#include "VulkanRenderPass.hpp"

using namespace giygas;


VulkanFramebuffer::VulkanFramebuffer(VulkanRenderer *renderer) {
    _renderer = renderer;
    _framebuffer = VK_NULL_HANDLE;
}

VulkanFramebuffer::~VulkanFramebuffer() {
    VkDevice device = _renderer->device();
    vkDestroyFramebuffer(device, _framebuffer, nullptr);
}

RendererType VulkanFramebuffer::renderer_type() const {
    return RendererType::Vulkan;
}

uint32_t VulkanFramebuffer::width() const {
    return _width;
}

uint32_t VulkanFramebuffer::height() const {
    return _height;
}

size_t VulkanFramebuffer::attachment_count() const {
    return _attachment_count;
}

void VulkanFramebuffer::create(const FramebufferCreateParameters &params) {
    assert(_framebuffer == VK_NULL_HANDLE);

    assert(params.pass != nullptr);
    assert(params.pass->renderer_type() == RendererType::Vulkan);
    const auto *pass_impl = reinterpret_cast<const VulkanRenderPass *>(params.pass);

    _width = params.width;
    _height = params.height;
    _attachment_count = params.attachment_count;

    unique_ptr<VkImageView[]> image_views(new VkImageView[params.attachment_count]);

    for (size_t i = 0; i < params.attachment_count; ++i) {
        const FramebufferAttachment &attachment = params.attachments[i];
        assert(attachment.target->renderer_type() == RendererType::Vulkan);
        const auto *target = static_cast<const VulkanRenderTarget *>(attachment.target->rendertarget_impl());
        image_views[i] = target->image_view();
    }

    VkFramebufferCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    create_info.renderPass = pass_impl->handle();
    create_info.attachmentCount = static_cast<uint32_t>(params.attachment_count);
    create_info.pAttachments = image_views.get();
    create_info.width = params.width;
    create_info.height = params.height;
    create_info.layers = 1;

    vkCreateFramebuffer(_renderer->device(), &create_info, nullptr, &_framebuffer);
}

VkFramebuffer VulkanFramebuffer::handle() const {
    return _framebuffer;
}

//VkRenderPass VulkanFramebuffer::renderpass() const {
//    return _pass;
//}
//
//const AttachmentPurpose *VulkanFramebuffer::purposes() const {
//    return _purposes.get();
//}
