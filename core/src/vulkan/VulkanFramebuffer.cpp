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

    unique_ptr<VkImageView[]> image_views(new VkImageView[params.attachment_count]);

    for (size_t i = 0; i < params.attachment_count; ++i) {
        const FramebufferAttachment &attacment = params.attachments[i];
        if (attacment.type == AttachmentType::Texture) {
            assert(attacment.texture->renderer_type() == RendererType::Vulkan);
            const auto *texture = reinterpret_cast<const VulkanTexture *>(attacment.texture);
            image_views[i] = texture->image_view();
        }
        else {
            assert(attacment.render_buffer->renderer_type() == RendererType::Vulkan);
            const auto *renderbuffer
                = reinterpret_cast<const VulkanRenderBuffer *>(attacment.render_buffer);
            image_views[i] = renderbuffer->image_view();
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

VkFramebuffer VulkanFramebuffer::handle() const {
    return _framebuffer;
}
