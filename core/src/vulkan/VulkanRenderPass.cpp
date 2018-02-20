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

    VkAttachmentReference color_ref = {};
    color_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depth_ref = {};
    depth_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription pass = {};
    pass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

    array<VkAttachmentDescription, 2> attachments = {};
    uint32_t attachment_count = 0;
    if (params.color_attachment.api_format != 0) {
        set_description_from_attachmnent_params(attachments[attachment_count], params.color_attachment);
        pass.colorAttachmentCount = 1;
        pass.pColorAttachments = &color_ref;
        color_ref.attachment = attachment_count;
        ++attachment_count;
    }
    if (params.depth_attachment.api_format != 0) {
        set_description_from_attachmnent_params(attachments[attachment_count], params.depth_attachment);
        pass.pDepthStencilAttachment = &depth_ref;
        depth_ref.attachment = attachment_count;
        ++attachment_count;
    }


    // TODO: More work is needed here to support stencil attachments.
    // In Vulkan, I don't think seperate attachments for depth and stencil work with just 1 subpass.
    // So, I either need to figure out how to automatically make 2 subpasses, or make the api
    // more similar to Vulkan (like I have done with the rest of giygas)


    VkRenderPassCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    create_info.attachmentCount = attachment_count;
    create_info.pAttachments = attachments.data();
    create_info.subpassCount = 1;
    create_info.pSubpasses = &pass;

    vkCreateRenderPass(_renderer->device(), &create_info, nullptr, &_handle);
}

VkRenderPass VulkanRenderPass::handle() const {
    return _handle;
}

void VulkanRenderPass::set_description_from_attachmnent_params(
    VkAttachmentDescription &description,
    const RenderPassAttachmentParameters &params
) {
    description.format = static_cast<VkFormat>(params.api_format);
    description.samples = VK_SAMPLE_COUNT_1_BIT;
    description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    description.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
}