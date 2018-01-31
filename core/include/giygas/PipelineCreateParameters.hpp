#pragma once
#include "Viewport.hpp"
#include "VertexAttributeLayout.hpp"
#include "ScissorRange.hpp"
#include "RenderPass.hpp"
#include "Shader.hpp"

namespace giygas {

    class PipelineCreateParameters {
    public:
        Viewport viewport;
        ScissorRange scissor;
        const VertexAttributeLayout* vertex_buffer_layouts;
        size_t vertex_buffer_layout_count;
        const RenderPass *render_pass;
        const Shader **shaders;
        size_t shader_count;
    };

}
