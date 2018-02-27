#include <giygas/validation/render_pass_validation.hpp>
#include "validation_common.hpp"

using namespace giygas;

bool validation::validate_render_pass_create(
    const RenderPass *target,
    const RenderPassCreateParameters &params
) {
    validate_begin("RenderPassCreate");
    validate(target != nullptr, "RenderPass being created is null.")
    validate(!target->is_valid(), "RenderPass being created has already been created.")

    size_t depth_stencil_attachment_count = 0;
    for (size_t i = 0; i < params.attachment_count; ++i) {
        const RenderPassAttachment &attachment = params.attachments[i];
        if (attachment.purpose == AttachmentPurpose::DepthStencil) {
            ++depth_stencil_attachment_count;
        }
    }
    // TODO: Make RenderPassCreateParameters to make it impossible to specify more than 1
    // depth/stencil attachment.
    validate(
        depth_stencil_attachment_count <= 1,
        "RenderPassCreateParams attachments cannot specify more than one depth/stencil attachment"
    );

    for (size_t i = 0; i < params.attachment_count; ++i) {
        const RenderPassAttachment &attachment = params.attachments[i];
        validate(attachment.target != nullptr, "Attachment #" << i << "'s pointer to render target is null.");
        validate(attachment.target->renderer_type() == target->renderer_type(), "Attachment #" << i << "'s target is of the incorrect renderer type.");
    }



    return true;
}
