#include "SetPipelineGLOperation.hpp"

using namespace giygas;

SetPipelineGLOperation::SetPipelineGLOperation() {
}

void SetPipelineGLOperation::set(PipelineOptions options) {
    _options = options;
}

void SetPipelineGLOperation::execute(GL *gl) {
    if (_options.polygon_culling.enabled) {
        gl->enable(GL_CULL_FACE);
    }
    else {
        gl->disable(GL_CULL_FACE);
    }

    gl->cull_face(_options.polygon_culling.mode == PolygonCullingMode::BackFace
        ? GL_BACK
        : GL_FRONT
    );

    gl->front_face(
        _options.polygon_culling.front_face_vertex_winding ==
            VertexWinding::CounterClockwise
        ? GL_CCW
        : GL_CW
    );

    if (_options.depth_buffer.depth_test_enabled) {
        gl->enable(GL_DEPTH_TEST);
    }
    else {
        gl->disable(GL_DEPTH_TEST);
    }

    gl->depth_mask(static_cast<GLboolean>(_options.depth_buffer.mask_enabled));
    gl->depth_func(depth_function_to_enum(_options.depth_buffer.function));
    gl->depth_range(
        static_cast<GLdouble>(_options.depth_buffer.range_near),
        static_cast<GLdouble>(_options.depth_buffer.range_far)
    );
}

GLenum SetPipelineGLOperation::depth_function_to_enum(DepthFunction function) {
    switch (function) {
        case DepthFunction::PassLess:
            return GL_LESS;
        case DepthFunction::PassAlways:
            return GL_ALWAYS;
        case DepthFunction::PassEqual:
            return GL_EQUAL;
        case DepthFunction::PassGreater:
            return GL_GREATER;
        case DepthFunction::PassGreaterThanOrEqual:
            return GL_GEQUAL;
        case DepthFunction::PassLessThanOrEqual:
            return GL_LEQUAL;
        case DepthFunction::PassNever:
            return GL_NEVER;
        case DepthFunction::PassNotEqual:
            return GL_NOTEQUAL;
    }
}