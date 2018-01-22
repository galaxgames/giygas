#include "PipelineEnableDisableGLOperation.hpp"

using namespace giygas;

PipelineEnableDisableGLOperation::PipelineEnableDisableGLOperation() {
}

void PipelineEnableDisableGLOperation::set(GLenum feature, bool value) {
    _feature = feature;
    _value = value;
}

void PipelineEnableDisableGLOperation::execute(GL *gl) {
    if (_value) {
        gl->enable(_feature);
    }
    else {
        gl->disable(_feature);
    }
}
