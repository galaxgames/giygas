#include <cassert>
#include "VulkanFramebuffer.hpp"
#include "VulkanRenderer.hpp"
#include "VulkanTexture.hpp"
#include "VulkanRenderBuffer.hpp"

using namespace giygas;


VulkanFramebuffer::VulkanFramebuffer(VulkanRenderer *renderer) {
    _renderer = renderer;
    _framebuffer = VK_NULL_HANDLE;
    _pass = VK_NULL_HANDLE;
}

VulkanFramebuffer::~VulkanFramebuffer() {
    VkDevice device = _renderer->device();
    vkDestroyFramebuffer(device, _framebuffer, nullptr);
    vkDestroyRenderPass(device, _pass, nullptr);
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

    _width = params.width;
    _height = params.height;
    _attachment_count = params.attachment_count;

    unique_ptr<VkAttachmentDescription[]> attachment_descriptions(
        new VkAttachmentDescription[params.attachment_count] {}
    );

    size_t color_attachment_count = 0;
    size_t depth_stencil_attachment_count = 0;

    for (size_t i = 0; i < params.attachment_count; ++i) {
        const FramebufferAttachment &attachment = params.attachments[i];
        if (attachment.purpose == AttachmentPurpose::Color) {
            ++color_attachment_count;
        }
        else {
            ++depth_stencil_attachment_count;
        }
    }

    assert(depth_stencil_attachment_count < 2);

    unique_ptr<VkAttachmentReference[]> color_refs(
        new VkAttachmentReference[color_attachment_count] {}
    );
    VkAttachmentReference depth_stencil_ref = {};
    depth_stencil_ref.attachment = VK_ATTACHMENT_UNUSED;

    _purposes = unique_ptr<AttachmentPurpose []>(
        new AttachmentPurpose[params.attachment_count] {}
    );

    size_t next_color_ref_index = 0;
    size_t next_depth_stencil_ref_index = 0;

    for (size_t i = 0; i < params.attachment_count; ++i) {
        const FramebufferAttachment &attachment = params.attachments[i];
        _purposes[i] = attachment.purpose;
        VkAttachmentDescription &description = attachment_descriptions[i];
        assert(attachment.target != nullptr);
        assert(attachment.target->renderer_type() == RendererType::Vulkan);
        const auto *target = static_cast<const VulkanRenderTarget *>(attachment.target->rendertarget_impl());
        set_description_from_attachmnent_params(description, target);

        VkAttachmentReference *ref;
        if (attachment.purpose == AttachmentPurpose::Color) {
            ref = &color_refs[next_color_ref_index++];
        }
        else {
            ref = &depth_stencil_ref;
        }

        ref->layout = target->layout();
        ref->attachment = static_cast<uint32_t>(i);
    }

    VkSubpassDescription pass_description = {};
    pass_description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    pass_description.colorAttachmentCount = static_cast<uint32_t>(color_attachment_count);
    pass_description.pColorAttachments = color_refs.get();
    pass_description.pDepthStencilAttachment = &depth_stencil_ref;

    VkRenderPassCreateInfo renderpass_create_info = {};
    renderpass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderpass_create_info.attachmentCount = static_cast<uint32_t>(params.attachment_count);
    renderpass_create_info.pAttachments = attachment_descriptions.get();
    renderpass_create_info.subpassCount = 1;
    renderpass_create_info.pSubpasses = &pass_description;

    vkCreateRenderPass(_renderer->device(), &renderpass_create_info, nullptr, &_pass);


    unique_ptr<VkImageView[]> image_views(new VkImageView[params.attachment_count]);

    for (size_t i = 0; i < params.attachment_count; ++i) {
        const FramebufferAttachment &attachment = params.attachments[i];
        assert(attachment.target->renderer_type() == RendererType::Vulkan);
        const auto *target = static_cast<const VulkanRenderTarget *>(attachment.target->rendertarget_impl());
        image_views[i] = target->image_view();
    }

    VkFramebufferCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    create_info.renderPass = _pass;
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

VkRenderPass VulkanFramebuffer::renderpass() const {
    return _pass;
}

const AttachmentPurpose *VulkanFramebuffer::purposes() const {
    return _purposes.get();
}

void VulkanFramebuffer::set_description_from_attachmnent_params(
    VkAttachmentDescription &description,
    const VulkanRenderTarget *target
) {
    description.format = static_cast<VkFormat>(target->api_format());
    description.samples = VK_SAMPLE_COUNT_1_BIT;
    description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
}