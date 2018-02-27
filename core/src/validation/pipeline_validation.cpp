#include <giygas/validation/pipeline_validation.hpp>
#include "validation_common.hpp"

using namespace giygas;

bool validation::validate_pipeline_create(
    const Pipeline *target,
    const PipelineCreateParameters &params
) {
    validate_begin("PipelineCreate");
    validate(target != nullptr, "Pipeline to create cannot be null.")
    validate(!target->is_valid(), "Pipeline has already been created.");

    if (params.descriptor_set != nullptr) {
        validate(
            params.descriptor_set->renderer_type() == target->renderer_type(),
            "RendererType of given DescriptorSet (params.descriptor_set) does not match Pipeline's "
            "RendererType."
        );
    }

    if (params.shader_count > 0) {
        validate(
            params.shaders != nullptr,
            "Pointer to Shaders cannot be null if params.shader_count is greater than zero."
        );
    }
    for (size_t i = 0; i < params.shader_count; ++i) {
        const Shader *shader = params.shaders[i];
        validate(
            shader != nullptr,
            "Given Shader #" << i << " (params.shaders[" << i << "] cannot be null."
        )
        validate(
            shader->renderer_type() == target->renderer_type(),
            "RendererType of given Shader #" << i << " (params.shaders[" << i << "] does not match "
            "the Pipeline's RendererType."
        );
    }

    validate(params.pass != nullptr, "Given RenderPass (params.pass) cannot be null.");
    validate(
        params.pass->renderer_type() == target->renderer_type(),
        "RendererType of given RendererPass (params.pass) does not match the Pipeline's "
        "RendererType."
    );

    return true;
}