#include <giygas/validation/submission_validation.hpp>
#include "validation_common.hpp"

using namespace giygas;

bool validate_draw_info(const DrawInfo &info, uint32_t index, RendererType renderer_type) {
    validate_begin("CommandBufferDrawInfo");
    validate(info.pipeline != nullptr, "DrawInfo[" << index << "]: Given pipeline cannot be null");

    validate(
        info.pipeline->renderer_type() == renderer_type,
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
            vertex_buffer != nullptr,
            "DrawInfo[" << index << "]: Given pointer to vertex buffer #" << i << " is null"
        )
        validate(
            vertex_buffer->is_valid(),
            "DrawInfo[" << index << "]: VertexBuffer #" << i << " is not in a valid state."
        )
        validate(
            vertex_buffer->renderer_type() == renderer_type,
            "DrawInfo[" << index << "]: RendererType of given vertex buffer #" << i << " does not "
            "match the RendererType of the CommandBuffer being recorded."
        );
    }

    validate(
        info.index_buffer != nullptr,
        "DrawInfo[" << index << "]: Given pointer to IndexBuffer must not be null."
    );
    validate(
        info.index_buffer->renderer_type() == renderer_type,
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
            info.descriptor_set->renderer_type() == renderer_type,
            "DrawInfo[" << index << "]: RendererType of the given DescriptorSet does not match the "
            "RendererType of the CommandBuffer being recorded."
        );
        validate(
            info.pipeline->is_descriptor_set_compatible(info.descriptor_set),
            "DrawInfo[" << index << "]: The given DescriptorSet is not compatible with the given "
            "Pipeline."
        );
    }

    // Validate push constants
    PushConstantsRange pipeline_pc_v = info.pipeline->vertex_push_constants_range();
    PushConstantsRange pipeline_pc_f = info.pipeline->fragment_push_constants_range();
    if (pipeline_pc_v.size > 0 || info.vertex_push_constants.range.size > 0) {
        validate(
            info.vertex_push_constants.range.size == pipeline_pc_v.size && info.vertex_push_constants.range.offset == pipeline_pc_v.offset,
            "DrawInfo[" << index << "]: The given vertex push constants range does not match the pipelines."
        )
        validate(
            info.vertex_push_constants.data != nullptr,
            "DrawInfo[" << index << "]: The given pointer vertex push constants data cannot be "
            "null when the given vertex push constant size is greater than zero."
        )
    }
    if (pipeline_pc_f.size > 0 || info.fragment_push_constants.range.size > 0) {
        validate(
            info.fragment_push_constants.range.size == pipeline_pc_f.size && info.fragment_push_constants.range.offset == pipeline_pc_f.offset,
            "DrawInfo[" << index << "]: The given fragment push constants range does not match the pipeline's."
        )
        validate(
            info.fragment_push_constants.data != nullptr,
            "DrawInfo[" << index << "]: The given pointer fragment push constants data cannot be "
                "null when the given fragment push constant size is greater than zero."
        )
    }

    return true;
}

bool validate_pass_info(const PassSubmissionInfo &pass, RendererType renderer_type) {
    validate_begin("CommandBuffersRecordPass");

    if (pass.draw_count > 0) {
        validate(
            pass.draws != nullptr
            , "Pointer to DrawInfos (pass.draws) cannot be null if pass.draw_count is greater than zero."
        );
    }

    validate(
        pass.pass_info.pass != nullptr,
        "Pointer to RenderPass for render pass info (pass.pass_info.pass) cannot be null."
    );
    validate(
        pass.pass_info.framebuffer != nullptr,
        "Pointer to Framebuffer for render pass info (pass.pass_info.framebuffer) cannot be null."
    );
    validate(
        pass.pass_info.pass->renderer_type() == renderer_type,
        "RendererType of RenderPass specified by render pass info (info.pass_info.pass) is not the correct "
        "renderer type."
    );
    validate(
        pass.pass_info.framebuffer->renderer_type() == RendererType::Vulkan,
        "RendererType of Framebuffer specified by render pass info (pass.pass_info.framebuffer) "
        "does not match the RendererType of CommandBuffer."
    );

    validate(
        pass.pass_info.clear_value_count >= pass.pass_info.framebuffer->attachment_count()
        , "Count of clear values specified in the given render pass info (" <<
          pass.pass_info.clear_value_count << ") must be greater than the attachment count of the "
          "framebuffer specified in the given render pass info (" <<
          pass.pass_info.framebuffer->attachment_count() << ")."
    );

    if (pass.pass_info.clear_value_count > 0) {
        validate(
            pass.pass_info.clear_values != nullptr,
            "Pointer to clear values specified by the given render pass info must not be null."
        )
    }

    // Validate that render pass and framebuffer are compatible.
    validate(
        pass.pass_info.pass->attachment_count() == pass.pass_info.framebuffer->attachment_count(),
        "RenderPass and Framebuffer specified in render pass info are incompatible, they have "
        "different attachment counts. The RenderPass contains " <<
        pass.pass_info.pass->attachment_count() << "while the Framebuffer contains " <<
        pass.pass_info.framebuffer->attachment_count() << "."
    )
    uint32_t attachment_count = pass.pass_info.pass->attachment_count();
    const AttachmentPurpose *pass_purposes = pass.pass_info.pass->attachment_purposes();
    const AttachmentPurpose *framebuffer_purposes = pass.pass_info.framebuffer->attachment_purposes();
    for (uint32_t i = 0; i < attachment_count; ++i) {
        validate(
            pass_purposes[i] == framebuffer_purposes[i],
            "Purpose of attachment #" << i << " varries between Framebuffer and Renderpass "
            "specified by given render pass info."
        );
    }


    for (size_t i = 0; i < attachment_count; ++i) {
        const ClearValue &clear_value = pass.pass_info.clear_values[i];
        validate(
            pass_purposes[i] == clear_value.purpose,
            "Purpose of clear value #" << i << " does not match purpose of RenderPass/Framebuffer "
            "attachment at that index."
        );
    }

    if (pass.draw_count > 0) {
        validate(
            pass.draws != nullptr,
            "Given pointer to DrawInfos to record must not be null when given draw count is "
            "greater than zero."
        );
    }
    for (uint32_t i = 0; i < pass.draw_count; ++i) {
        if (!validate_draw_info(pass.draws[i], i, renderer_type)) {
            return false;
        }
    }

    return true;
}

bool validation::validate_submission_passes(
    const PassSubmissionInfo *passes
    , uint32_t pass_count
    , RendererType renderer_type
) {
    for (uint32_t i = 0; i < pass_count; ++i) {
        if (!validate_pass_info(passes[i], renderer_type)) {
            return false;
        }
    }
    return true;
}
