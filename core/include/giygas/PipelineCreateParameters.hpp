#pragma once
#include "Viewport.hpp"
#include "VertexAttributeLayout.hpp"
#include "ScissorRange.hpp"
#include "Shader.hpp"
#include "DescriptorSet.hpp"
#include "Framebuffer.hpp"

namespace giygas {

    class PushConstantsRange {
    public:
        size_t size;
        size_t offset;
    };

    class PipelineCreateParameters {
    public:
        Viewport viewport;
        ScissorRange scissor;
        const VertexAttributeLayout* vertex_buffer_layouts;
        size_t vertex_buffer_layout_count;
        const Framebuffer *framebuffer;
        const Shader **shaders;
        size_t shader_count;
        PushConstantsRange vertex_push_constants;
        PushConstantsRange fragment_push_constants;
        const DescriptorSet *descriptor_set;
    };

}
