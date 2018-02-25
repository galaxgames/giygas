#pragma once
#include "Viewport.hpp"
#include "VertexAttributeLayout.hpp"
#include "ScissorRange.hpp"
#include "Shader.hpp"
#include "DescriptorSet.hpp"
#include "Framebuffer.hpp"
#include "ColorChannels.hpp"

namespace giygas {

    class PushConstantsRange {
    public:
        size_t size;
        size_t offset;
    };

    enum class BlendOperation {
        ADD,
        SUBTRACT,
        REVERSE_SUBTRACT,
        MIN,
        MAX
    };

    enum class BlendFactor {
       ZERO,
       ONE,
       SRC_COLOR,
       ONE_MINUS_SRC_COLOR,
       DST_COLOR,
       ONE_MINUS_DST_COLOR,
       SRC_ALPHA,
       ONE_MINUS_SRC_ALPHA,
       DST_ALPHA,
       ONE_MINUS_DST_ALPHA,
       CONSTANT_COLOR,
       ONE_MINUS_CONSTANT_COLOR,
       CONSTANT_ALPHA,
       ONE_MINUS_CONSTANT_ALPHA,
       SRC_ALPHA_SATURATE,
       SRC1_COLOR,
       ONE_MINUS_SRC1_COLOR,
       SRC1_ALPHA,
       ONE_MINUS_SRC1_ALPHA
    };

    // COLOR BLENDING CHEATSHEET
    // if (blendEnable) {
    //     finalColor.rgb = (srcColorBlendFactor * newColor.rgb) <colorBlendOp> (dstColorBlendFactor * oldColor.rgb);
    //     finalColor.a = (srcAlphaBlendFactor * newColor.a) <alphaBlendOp> (dstAlphaBlendFactor * oldColor.a);
    // } else {
    //     finalColor = newColor;
    // }
    //
    // finalColor = finalColor & colorWriteMask;

    class BlendParameters {
    public:
        bool enabled;
        ColorChannels mask_channels;
        BlendFactor src_color_factor;
        BlendFactor dst_color_factor;
        BlendOperation color_op;
        BlendFactor src_alpha_factor;
        BlendFactor dst_alpha_factor;
        BlendOperation alpha_op;

        static GIYGAS_EXPORT BlendParameters conventional_alpha_blending();
        static GIYGAS_EXPORT BlendParameters premultiplied_alpha_blending();
    };

    class PipelineCreateParameters {
    public:
        Viewport viewport;
        ScissorRange scissor;
        BlendParameters blend;
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
