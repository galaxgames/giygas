#include <giygas/PipelineCreateParameters.hpp>

using namespace giygas;

BlendParameters BlendParameters::conventional_alpha_blending() {
    BlendParameters blend = {};
    blend.enabled = true;
    blend.color_op = BlendOperation::ADD;
    blend.src_color_factor = BlendFactor::SRC_ALPHA;
    blend.dst_color_factor = BlendFactor::ONE_MINUS_SRC_ALPHA;
    blend.alpha_op = BlendOperation::ADD;
    blend.src_alpha_factor = BlendFactor::ONE;
    blend.dst_alpha_factor = BlendFactor::ZERO;
    return blend;
}

BlendParameters BlendParameters::premultiplied_alpha_blending() {
    BlendParameters blend = {};
    blend.enabled = true;
    blend.color_op = BlendOperation::ADD;
    blend.src_color_factor = BlendFactor::ONE;
    blend.dst_color_factor = BlendFactor::ONE_MINUS_SRC_ALPHA;
    blend.alpha_op = BlendOperation::ADD;
    blend.src_alpha_factor = BlendFactor::ONE;
    blend.dst_alpha_factor = BlendFactor::ZERO;
    return blend;
}