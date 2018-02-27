#include <giygas/validation/command_buffer_validation.hpp>
#include "validation_common.hpp"

using namespace giygas;

bool validate_draw_info(const CommandBuffer *target, const DrawInfo &info, size_t index) {
    validate_begin("CommandBufferDrawInfo");
    validate(info.pipeline != nullptr, "DrawInfo[" << index << "]: Given pipeline cannot be null");

    validate(
        info.pipeline->renderer_type() == target->renderer_type(),
        "DrawInfo[" << index << "]: RendererType of given pipeline does not match the RendererType "
        "of the CommandBuffer being recorded."
    );

    if (info.vertex_buffer_count > 0) {
        validate(
            info.vertex_buffers != nullptr,
            "DrawInfo[" << index << "]: Given pointer to VertexBuffers cannot be null if given "
            "vertex buffer count is greater than zero."
        )
    }
    for (size_t i = 0; i < info.vertex_buffer_count; ++i) {
        const VertexBuffer *vertex_buffer = info.vertex_buffers[i];
        validate(
            vertex_buffer->renderer_type() == target->renderer_type(),
            "DrawInfo[" << index << "]: RendererType of given vertex buffer #" << i << " does not "
            "match the RendererType of the CommandBuffer being recorded."
        );
    }

    validate(
        info.index_buffer != nullptr,
        "DrawInfo[" << index << "]: Given pointer to IndexBuffer must not be null."
    );
    validate(
        info.index_buffer->renderer_type() == target->renderer_type(),
        "DrawInfo[" << index << "]: RendererType of given IndexBuffer does not match the "
        "RendererType of the CommandBuffer being recorded."
    );

    // Validate descriptor set
    if (info.pipeline->descriptor_set_count() > 0) {
        validate(
            info.descriptor_set != nullptr,
            "DrawInfo[" << index << "]: The given DescriptorSet must not be null "
            " when the given Pipeline uses descriptor sets."
        );
        validate(
            info.descriptor_set->renderer_type() == target->renderer_type(),
            "DrawInfo[" << index << "]: RendererType of the given DescriptorSet does not match the "
            "RendererType of the CommandBuffer being recorded."
        );
        validate(
            info.pipeline->is_descriptor_set_compatible(info.descriptor_set),
            "DrawInfo[" << index << "]: The given DescriptorSet is not compatible with the given "
            "Pipeline."
        );
    }

    return true;
}

bool validation::validate_command_buffer_create(
    const CommandBuffer *target,
    const CommandPool *pool
) {
    validate_begin("CommandBufferCreate");
    validate(target != nullptr, "CommandBuffer being created is null.")
    validate(pool != nullptr, "Given pointer to the owning CommandPool is null.")
    validate(pool->is_valid(), "Given CommandPool to own this CommandBuffer is not valid.")
    validate(pool->renderer_type() == target->renderer_type(), "Given CommandPool to own this CommandBuffer is of the incorrect renderer type.")
    return true;
}

bool validation::validate_command_buffer_record_pass(
    const CommandBuffer *target,
    const SingleBufferPassInfo &info
) {
    validate_begin("CommandBuffersRecordPass");
    validate(target != nullptr, "CommandBuffer target is null.")

    if (info.draw_count > 0) {
        validate(
            info.draws != nullptr,
            "Pointer to DrawInfos (info.draws) cannot be null if info.draw_count is greater than "
            "zero."
        );
    }

    validate(
        info.pass_info.pass != nullptr,
        "Pointer to RenderPass for render pass info (info.pass_info.pass) cannot be null."
    );
    validate(
        info.pass_info.framebuffer != nullptr,
        "Pointer to Framebuffer for render pass info (info.pass_info.framebuffer) cannot be null."
    );
    validate(
        info.pass_info.pass->renderer_type() == target->renderer_type(),
        "RendererType of RenderPass specified by render pass info (info.pass_info.pass) does not "
        "match the RendererType of CommandBuffer."
    );
    validate(
        info.pass_info.framebuffer->renderer_type() == RendererType::Vulkan,
        "RendererType of Framebuffer specified by render pass info (info.pass_info.framebuffer) "
        "does not match the RendererType of CommandBuffer."
    );

    validate(
        info.pass_info.clear_value_count >= info.pass_info.framebuffer->attachment_count(),
        "Count of clear values specified in the given render pass info (" <<
        info.pass_info.clear_value_count << ") must be greater than the attachment count of the "
        "framebuffer specified in the given render pass info (" <<
        info.pass_info.framebuffer->attachment_count() << ")."
    );

    if (info.pass_info.clear_value_count > 0) {
        validate(
            info.pass_info.clear_values != nullptr,
            "Pointer to clear values specified by the given render pass info must not be null."
        )
    }

    // Validate that render pass and framebuffer are compatible.
    validate(
        info.pass_info.pass->attachment_count() == info.pass_info.framebuffer->attachment_count(),
        "RenderPass and Framebuffer specified in render pass info are incompatible, they have "
        "different attachment counts. The RenderPass contains " <<
        info.pass_info.pass->attachment_count() << "while the Framebuffer contains " <<
        info.pass_info.framebuffer->attachment_count() << "."
    )
    size_t attachment_count = info.pass_info.pass->attachment_count();
    const AttachmentPurpose *pass_purposes = info.pass_info.pass->attachment_purposes();
    const AttachmentPurpose *framebuffer_purposes = info.pass_info.framebuffer->attachment_purposes();
    for (int i = 0; i < attachment_count; ++i) {
        validate(
            pass_purposes[i] == framebuffer_purposes[i],
            "Purpose of attachment #" << i << " varries between Framebuffer and Renderpass "
            "specified by given render pass info."
        );
    }


    for (size_t i = 0; i < attachment_count; ++i) {
        const ClearValue &clear_value = info.pass_info.clear_values[i];
        validate(
            pass_purposes[i] == clear_value.purpose,
            "Purpose of clear value #" << i << " does not match purpose of RenderPass/Framebuffer "
            "attachment at that index."
        );
    }

    if (info.draw_count > 0) {
        validate(
            info.draws != nullptr,
            "Given pointer to DrawInfos to record must not be null when given draw count is "
            "greater than zero."
        );
    }
    for (size_t i = 0; i < info.draw_count; ++i) {
        if (!validate_draw_info(target, info.draws[i], i)) {
            return false;
        }
    }

    return true;
}
