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
    vkDestroyFramebuffer(_renderer->device(), _framebuffer, nullptr);
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

void VulkanFramebuffer::create(const FramebufferCreateParameters &params) {
    _width = params.width;
    _height = params.height;

    unique_ptr<VkImageView[]> image_views(new VkImageView[params.attachment_count]);

    for (size_t i = 0; i < params.attachment_count; ++i) {
        const FramebufferAttachment &attachment = params.attachments[i];
        assert(attachment.target->renderer_type() == RendererType::Vulkan);
        const auto *target = static_cast<const VulkanRenderTarget *>(attachment.target->impl());
        image_views[i] = target->image_view();
    }

    assert(params.pass->renderer_type() == RendererType::Vulkan);
    const auto *pass = reinterpret_cast<const VulkanRenderPass *>(params.pass);

    VkFramebufferCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    create_info.renderPass = pass->handle();
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
