#include <giygas/validation/framebuffer_validation.hpp>
#include "validation_common.hpp"

using namespace giygas;

bool validation::validate_framebuffer_create(
    const Framebuffer *target,
    const FramebufferCreateParameters &params
) {
    validate_begin("FramebufferCreate");
    validate(target != nullptr, "Framebuffer being created cannot be null.")
    validate(!target->is_valid(), "Framebuffer has already been created.");
    validate(params.pass != nullptr, "Pointer to compatible RenderPass (params.pass) cannot be null.");
    validate(params.pass->is_valid(), "Given compatible RenderPass (params.pass) is not in a valid state. Did you forget to creating it?");
    validate(params.pass->renderer_type() == target->renderer_type(), "RendererType of given compatible RenderPass (params.pass) does not match the Framebuffers RendererType.")

    for (size_t i = 0; i < params.attachment_count; ++i) {
        const FramebufferAttachment &attachment = params.attachments[i];
        validate(
            attachment.target->renderer_type() == target->renderer_type(),
            "RendererType of given target for attachment #" << i <<
            " (params.attachments[" << i << "].target) does not match the Framebuffer's "
            "RendererType."
        );
    }

    // Validate that render pass and framebuffer are compatible.
    validate(
        params.pass->attachment_count() == params.attachment_count,
        "Attachment count specified by create parameters differs from count of attachments of "
        "RenderPass specified by create parameters. The RenderPass contains " <<
        params.pass->attachment_count() << "while the create parameters specify " <<
        params.attachment_count << "."
    )
    if (params.attachment_count > 0) {
        validate(
            params.attachments != nullptr,
            "Pointer to attachments cannot be null if given attachment count is not zero."
        )
    }
    size_t attachment_count = params.attachment_count;
    const AttachmentPurpose *pass_purposes = params.pass->attachment_purposes();
    for (int i = 0; i < attachment_count; ++i) {
        validate(
            pass_purposes[i] == params.attachments[i].purpose,
            "Purpose of attachment #" << i << " varries between Framebuffer create parameters and  "
            "Renderpass specified in create parameters."
        );
    }

    return true;
}