#include "ClearGLOperation.hpp"

using namespace giygas;

void ClearGLOperation::set(GLuint framebuffer, GLbitfield mask) {
    _framebuffer = framebuffer;
    _mask = mask;
}

void ClearGLOperation::set_color(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    _color_r = r;
    _color_g = g;
    _color_b = b;
    _color_a = a;
}

void ClearGLOperation::set_depth(GLdouble depth) {
    _depth = depth;
}

void ClearGLOperation::set_stencil(GLint stencil) {
    _stencil = stencil;
}

void ClearGLOperation::execute(GL *gl) {
    gl->bind_framebuffer(GL_FRAMEBUFFER, _framebuffer);
    gl->clear_color(_color_r, _color_g, _color_b, _color_a);
    gl->clear_depth(_depth);
    gl->clear_stencil(_stencil);
    gl->clear(_mask);
}
