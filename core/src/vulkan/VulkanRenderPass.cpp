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

    VkAttachmentDescription color_attachment = {};
    color_attachment.format = static_cast<VkFormat>(params.color_attachment.api_format);
    color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_ref = {};
    color_ref.attachment = 0;
    color_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription pass = {};
    pass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    pass.colorAttachmentCount = 1;
    pass.pColorAttachments = &color_ref;

    VkRenderPassCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    create_info.attachmentCount = 1;
    create_info.pAttachments = &color_attachment;
    create_info.subpassCount = 1;
    create_info.pSubpasses = &pass;

    vkCreateRenderPass(_renderer->device(), &create_info, nullptr, &_handle);
}

VkRenderPass VulkanRenderPass::handle() const {
    return _handle;
}