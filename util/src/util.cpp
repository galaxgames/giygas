#include "../include/giygasutil/util.hpp"

using namespace giygasutil;

template <typename PtrT>
PtrT *util::create_swapchain_framebuffers(Renderer *renderer, const RenderPass *pass) {
    size_t framebuffer_count = renderer->swapchain_framebuffer_count();
    PtrT *framebuffers = new PtrT[framebuffer_count];
    for (uint32_t i = 0; i < framebuffer_count; ++i) {
        PtrT &framebuffer = framebuffers[i];

        FramebufferAttachment attachment = {};
        attachment.purpose = AttachmentPurpose::Color;
        attachment.target = renderer->get_swapchain_rendertarget(i);

        FramebufferCreateParameters fb_params = {};
        fb_params.width = renderer->swapchain_width();
        fb_params.height = renderer->swapchain_height();
        fb_params.attachment_count = 1;
        fb_params.attachments = &attachment;
        fb_params.pass = pass;

        framebuffer = PtrT(renderer->make_framebuffer());
        framebuffer->create(fb_params);
    }
    return framebuffers;
}

namespace giygasutil {
namespace util {
    template Framebuffer **create_swapchain_framebuffers<Framebuffer *>(Renderer *, const RenderPass *);
    template unique_ptr<Framebuffer> *create_swapchain_framebuffers<unique_ptr<Framebuffer>>(Renderer *, const RenderPass *);
}}