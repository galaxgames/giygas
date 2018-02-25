#include "VulkanRenderPass.hpp"
#include "VulkanRenderer.hpp"

using namespace giygas;

VulkanRenderPass::VulkanRenderPass(VulkanRenderer *renderer) {
    _renderer = renderer;
    _handle = VK_NULL_HANDLE;
}

VulkanRenderPass::~VulkanRenderPass() {
    vkDestroyRenderPass(_renderer->device(), _handle, nullptr);
}

RendererType VulkanRenderPass::renderer_type() const {
    return RendererType::Vulkan;
}

void VulkanRenderPass::create(const RenderPassCreateParameters &params) {

    unique_ptr<VkAttachmentDescription[]> attachment_descriptions(
        new VkAttachmentDescription[params.attachment_count] {}
    );

    size_t color_attachment_count = 0;
    size_t depth_stencil_attachment_count = 0;

    for (size_t i = 0; i < params.attachment_count; ++i) {
        const RenderPassAttachment &attachment = params.attachments[i];
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
        const RenderPassAttachment &attachment = params.attachments[i];
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

    vkCreateRenderPass(_renderer->device(), &renderpass_create_info, nullptr, &_handle);
}

VkRenderPass VulkanRenderPass::handle() const {
    return _handle;
}

const AttachmentPurpose* VulkanRenderPass::purposes() const {
    return _purposes.get();
}

void VulkanRenderPass::set_description_from_attachmnent_params(
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