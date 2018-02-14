#include <cassert>
#include "VulkanFramebuffer.hpp"
#include "VulkanRenderer.hpp"
#include "VulkanTexture.hpp"
#include "VulkanRenderBuffer.hpp"
#include "VulkanRenderPass.hpp"

using namespace giygas;


VulkanFramebuffer::VulkanFramebuffer(
    VulkanRenderer *renderer,
    const FramebufferCreateParameters &params
) {
    _renderer = renderer;
    _width = params.width;
    _height = params.height;

    unique_ptr<VkImageView[]> image_views(new VkImageView[params.attachment_count]);

    for (size_t i = 0; i < params.attachment_count; ++i) {
        const FramebufferAttachment &attachment = params.attachments[i];
        switch (attachment.type)
        {
            case AttachmentType::Texture:
            {
                assert(attachment.texture->renderer_type() == RendererType::Vulkan);
                const auto *texture = reinterpret_cast<const VulkanTexture *>(attachment.texture);
                image_views[i] = texture->image_view();
                break;
            }
            case AttachmentType::RenderBuffer:
            {
                assert(attachment.render_buffer->renderer_type() == RendererType::Vulkan);
                const auto *renderbuffer
                    = reinterpret_cast<const VulkanRenderBuffer *>(attachment.render_buffer);
                image_views[i] = renderbuffer->image_view();
                break;
            }
        }
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

    vkCreateFramebuffer(renderer->device(), &create_info, nullptr, &_framebuffer);
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

VkFramebuffer VulkanFramebuffer::handle() const {
    return _framebuffer;
}
