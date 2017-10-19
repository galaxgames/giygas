#include <cassert>
#include <glad/glad.h>
#include <giygas_internal/GLVertexArray.hpp>
#include <giygas_internal/GLRenderer.hpp>
#include <giygas_internal/GLVertexBuffer.hpp>
#include <giygas_internal/GLElementBuffer.hpp>
#include <giygas_internal/GLMaterial.hpp>
#include <giygas_internal/GLTexture.hpp>
#include <giygas_internal/GLFrameBufferSurface.hpp>
#include <giygas_internal/GLRenderBuffer.hpp>

using namespace std;
using namespace giygas;

GLRenderer::GLRenderer(shared_ptr<Window> window)
    : _main_surface(&_gl)
    , _window(move(window))
{
    gladLoadGL();
    _window->add_surface_size_changed_listener(this);
    _main_surface.set_size(
        _window->framebuffer_width(),
        _window->framebuffer_height()
    );

    // Set up some defaults that should be configurable later
    glEnable(GL_CULL_FACE);
}

GLRenderer::GLRenderer(GLRenderer &&other) noexcept
    : _gl(move(other._gl))
    , _main_surface(move(other._main_surface))
    , _window(move(other._window))
{
    move_common(move(other));
}

GLRenderer& GLRenderer::operator=(GLRenderer &&other) noexcept {
    _gl = move(other._gl);
    _main_surface = move(other._main_surface);
    _window = move(other._window);
    move_common(move(other));
    return *this;
}

void GLRenderer::move_common(GLRenderer &&other) noexcept {
    _window->remove_surface_size_changed_listener(&other);
    _window->add_surface_size_changed_listener(this);
}

GLRenderer::~GLRenderer() = default;

VertexBuffer *GLRenderer::make_vbo() {
    return new GLVertexBuffer(&_gl);
}

VertexArray *GLRenderer::make_vao() {
    return new GLVertexArray(&_gl);
}

ElementBuffer<unsigned int> *GLRenderer::make_int_ebo() {
    return new GLElementBuffer<unsigned int>(&_gl);
}

ElementBuffer<unsigned short> *GLRenderer::make_short_ebo() {
    return new GLElementBuffer<unsigned short>(&_gl);
}

ElementBuffer<unsigned char> *GLRenderer::make_char_ebo() {
    return new GLElementBuffer<unsigned char>(&_gl);
}

Material *GLRenderer::make_material() {
    return new GLMaterial(&_gl);
}

Shader *GLRenderer::make_shader() {
    return new GLShader(&_gl);
}

Texture *GLRenderer::make_texture() {
    return new GLTexture(&_gl);
}

FrameBufferSurface *GLRenderer::make_framebuffer() {
    return new GLFrameBufferSurface(&_gl);
}

RenderBuffer *GLRenderer::make_renderbuffer() {
    return new GLRenderBuffer(&_gl);
}

Surface *GLRenderer::main_surface() {
    return &_main_surface;
}

void GLRenderer::handle_surface_size_changed(
    unsigned int width,
    unsigned int height
) {
    _main_surface.set_size(width, height);
}
