#include <cassert>
#include <giygas/validation/framebuffer_validation.hpp>
#include "VulkanFramebuffer.hpp"
#include "VulkanRenderer.hpp"
#include "VulkanTexture.hpp"
#include "VulkanRenderPass.hpp"

using namespace giygas;
using namespace giygas::validation;


class FramebufferSafeDeletable final : public SwapchainSafeDeleteable {

    uint32_t _handle_count = 0;
    unique_ptr<VkFramebuffer[]> _handles;

public:

    FramebufferSafeDeletable(uint32_t handle_count, unique_ptr<VkFramebuffer[]> handles) {
        _handle_count = handle_count;
        _handles = move(handles);
    }

    void delete_resources(VulkanRenderer &renderer) override {
        VkDevice device = renderer.device();

        for (uint32_t i = 0; i < _handle_count; ++i) {
            vkDestroyFramebuffer(device, _handles[i], nullptr);
        }
    }

};


VulkanFramebuffer::VulkanFramebuffer(VulkanRenderer *renderer) {
    _renderer = renderer;
}

VulkanFramebuffer::~VulkanFramebuffer() {
    auto * deletable = new FramebufferSafeDeletable(_handle_count, move(_handles));
    _renderer->delete_when_safe(unique_ptr<SwapchainSafeDeleteable>(deletable));
}

RendererType VulkanFramebuffer::renderer_type() const {
    return RendererType::Vulkan;
}

void VulkanFramebuffer::create(const FramebufferCreateParameters &params) {
    assert(validate_framebuffer_create(this, params));
    // TODO: Validate that all attachments are either for or not for the swapchain.

    const auto *pass_impl = reinterpret_cast<const VulkanRenderPass *>(params.pass);

    _width = params.width;
    _height = params.height;
    _attachment_count = params.attachment_count;
    _purposes = unique_ptr<AttachmentPurpose[]>(new AttachmentPurpose[params.attachment_count] {});

    _handle_count = 1;

    // Determine if we need to allocate multiple framebuffers due to swapchain render target attachments
    for (uint32_t i = 0; i < params.attachment_count; ++i) {
        const FramebufferAttachment &attachment = params.attachments[i];
        const auto *target = static_cast<const VulkanRenderTarget *>(attachment.target->rendertarget_impl());
        if (target->is_swapchain()) {
            _handle_count = _renderer->swapchain_image_count();
            break;
        }
    }

    // Create all (or just the one) vulkan framebuffer objects.
    _handles = unique_ptr<VkFramebuffer[]>(new VkFramebuffer[_handle_count]);

    for (uint32_t framebuffer_index = 0; framebuffer_index < _handle_count; ++framebuffer_index) {
        unique_ptr<VkImageView[]> image_views(new VkImageView[params.attachment_count]);

        for (uint32_t i = 0; i < params.attachment_count; ++i) {
            const FramebufferAttachment &attachment = params.attachments[i];
            const auto *target = static_cast<const VulkanRenderTarget *>(attachment.target->rendertarget_impl());
            image_views[i] = target->image_view(framebuffer_index);
            _purposes[i] = attachment.purpose;
            _is_for_swapchain = target->is_swapchain();
        }

        VkFramebufferCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        create_info.renderPass = pass_impl->handle();
        create_info.attachmentCount = static_cast<uint32_t>(params.attachment_count);
        create_info.pAttachments = image_views.get();
        create_info.width = params.width;
        create_info.height = params.height;
        create_info.layers = 1;

        vkCreateFramebuffer(_renderer->device(), &create_info, nullptr, _handles.get() + framebuffer_index);
    }
}

bool VulkanFramebuffer::is_valid() const {
    return _handle_count > 0;
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

const AttachmentPurpose* VulkanFramebuffer::attachment_purposes() const {
    return _purposes.get();
}

bool VulkanFramebuffer::is_for_swapchain() const {
    return _is_for_swapchain;
}

VkFramebuffer VulkanFramebuffer::get_handle(uint32_t index) const {
    assert(index < _handle_count);
    return _handles[index];
}
