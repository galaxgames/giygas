#include <giygas_internal/GLMainSurface.hpp>

using namespace giygas;
using namespace std;

GLMainSurface::GLMainSurface(GL *gl) {
    _gl = gl;
}

GLMainSurface::GLMainSurface(GLMainSurface &&other) noexcept
    : _renderer(move(other._renderer))
{
    _gl = other._gl;
}

GLMainSurface &GLMainSurface::operator=(GLMainSurface &&other) noexcept {
    _renderer = other._renderer;
    _gl = other._gl;
    return *this;
}

void GLMainSurface::set_clear_color(Vector4 color) {
    _renderer.set_clear_color(color);
}

void GLMainSurface::set_clear_depth(double value) {
    _renderer.set_clear_depth(value);
}

void GLMainSurface::set_clear_stencil(int value) {
    _renderer.set_clear_stencil(value);
}

void GLMainSurface::clear(SurfaceBufferType surfaces) {
    _gl->bind_framebuffer(GL_DRAW_FRAMEBUFFER, 0);
    _renderer.clear(surfaces);
}

void GLMainSurface::draw(
    VertexArray *vao,
    ElementBuffer<unsigned int> *ebo,
    Material *material,
    ElementDrawInfo element_info
) {
    _gl->bind_framebuffer(GL_DRAW_FRAMEBUFFER, 0);
    _renderer.draw(_gl, vao, ebo, material, element_info);
}

void GLMainSurface::draw(
    VertexArray *vao,
    ElementBuffer<unsigned short> *ebo,
    Material *material,
    ElementDrawInfo element_info
) {
    _gl->bind_framebuffer(GL_DRAW_FRAMEBUFFER, 0);
    _renderer.draw(_gl, vao, ebo, material, element_info);
}

void GLMainSurface::draw(
    VertexArray *vao,
    ElementBuffer<unsigned char> *ebo,
    Material *material,
    ElementDrawInfo element_info
) {
    _gl->bind_framebuffer(GL_DRAW_FRAMEBUFFER, 0);
    _renderer.draw(_gl, vao, ebo, material, element_info);
}
