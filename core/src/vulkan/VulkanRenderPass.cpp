#include "VulkanRenderPass.hpp"vk
#include "VulkanRenderer.hpp"

using namespace giygas;

VulkanRenderPass::VulkanRenderPass(VulkanRenderer *renderer) {
    _renderer = renderer;
    _handle = VK_NULL_HANDLE;
}

VulkanRenderPass::~VulkanRenderPass() {

}

void VulkanRenderPass::create(const RenderPassCreateParameters &params) {

}