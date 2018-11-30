#include "../include/giygasutil/util.hpp"

using namespace giygasutil;
using namespace giygas;


void giygasutil::create_basic_framebuffer(
    Framebuffer *framebuffer,
    const RenderTarget *rendertarget,
    const RenderPass *pass
) {
    FramebufferAttachment attachment = {};
    attachment.purpose = AttachmentPurpose::Color;
    attachment.target = rendertarget;

    FramebufferCreateParameters fb_params = {};
    fb_params.width = rendertarget->width();
    fb_params.height = rendertarget->height();
    fb_params.attachment_count = 1;
    fb_params.attachments = &attachment;
    fb_params.pass = pass;

    framebuffer->create(fb_params);
}
