#include "SetTextureParameterGLOperation.hpp"

using namespace giygas;
using namespace std;

SetTextureParameterGLOperation::SetTextureParameterGLOperation(
) {
}

void SetTextureParameterGLOperation::set(
    GLuint handle,
    GLenum target,
    GLenum parameter,
    GLint value
) {
    _handle = handle;
    _target = target;
    _parameter = parameter;
    _value = value;
}

void SetTextureParameterGLOperation::execute(GL *gl) {
    gl->bind_texture(_target, _handle);
    gl->tex_parameter_i(_target, _parameter, _value);
}
