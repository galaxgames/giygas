#include <cassert>
#include "GLMainSurface.hpp"
#include "GLRenderer.hpp"

using namespace giygas;
using namespace std;

GLMainSurface::GLMainSurface() {
    _renderer = nullptr;
};

GLMainSurface::GLMainSurface(GLRenderer *renderer) {
    _renderer = renderer;
}

GLMainSurface::GLMainSurface(GLMainSurface &&other) noexcept
    : _surface_renderer(move(other._surface_renderer))
{
    assert(other._renderer != nullptr);
    _renderer = other._renderer;
}

GLMainSurface &GLMainSurface::operator=(GLMainSurface &&other) noexcept {
    assert(other._renderer != nullptr);
    _surface_renderer = other._surface_renderer;
    _renderer = other._renderer;
    return *this;
}

unsigned int GLMainSurface::width() const {
    return _width;
}

unsigned int GLMainSurface::height() const {
    return _height;
}

void GLMainSurface::set_viewport(
    unsigned int x,
    unsigned int y,
    unsigned int width,
    unsigned int height
) {
    _surface_renderer.set_viewport(x, y, width, height);
}

void GLMainSurface::set_clear_color(Vector4 color) {
    _surface_renderer.set_clear_color(color);
}

void GLMainSurface::set_clear_depth(double value) {
    _surface_renderer.set_clear_depth(value);
}

void GLMainSurface::set_clear_stencil(int value) {
    _surface_renderer.set_clear_stencil(value);
}

void GLMainSurface::clear(SurfaceBufferType surfaces) {
    assert(_renderer != nullptr);
    _surface_renderer.clear(_renderer, 0, surfaces);
}

#define GIYGAS_MAIN_SURFACE_DRAW_FUNC_IMPL(ebo_type) \
void GLMainSurface::draw( \
    VertexArray *vao, \
    ElementBuffer<ebo_type> *ebo, \
    Material *material, \
    ElementDrawInfo element_info \
) { \
    assert(_renderer != nullptr); \
    _surface_renderer.draw(_renderer, 0, vao, ebo, material, element_info); \
}

GIYGAS_MAIN_SURFACE_DRAW_FUNC_IMPL(unsigned int)
GIYGAS_MAIN_SURFACE_DRAW_FUNC_IMPL(unsigned short)
GIYGAS_MAIN_SURFACE_DRAW_FUNC_IMPL(unsigned char)

void GLMainSurface::set_size(unsigned int width, unsigned int height) {
    _width = width;
    _height = height;
}
