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
#ifndef NDEBUG
    _initialized = false;
#endif
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
#ifndef NDEBUG
    _initialized = other._initialized;
    other._initialized = false;
#endif
}

GLRenderer::~GLRenderer() {
    // TODO: Utilize RAII for listeners.
    _window->remove_surface_size_changed_listener(this);
}

void GLRenderer::initialize(RendererInitOptions options) {
    assert(_window->is_valid());

    // TODO: Stop using glad so we can support having more than GLRenderer
    // instance
    gladLoadGL();
    _window->add_surface_size_changed_listener(this);
    _main_surface.set_size(
        _window->framebuffer_width(),
        _window->framebuffer_height()
    );

    set_polygon_culling_enabled(options.polygon_culling_enabled);
    set_polygon_culling_mode(options.polygon_culling_mode);
    set_front_face_winding(options.front_face_vertex_winding);

#ifndef NDEBUG
    _initialized = true;
#endif
}

void GLRenderer::set_polygon_culling_enabled(bool value) {
    if (value) {
        glEnable(GL_CULL_FACE);
    }
    else {
        glDisable(GL_CULL_FACE);
    }
}

void GLRenderer::set_polygon_culling_mode(PolygonCullingMode value) {
    glCullFace(value == PolygonCullingMode::BackFace ? GL_BACK : GL_FRONT);
}

void GLRenderer::set_front_face_winding(VertexWinding value) {
    glFrontFace(value == VertexWinding::CounterClockwise ? GL_CCW : GL_CW);
}

VertexBuffer *GLRenderer::make_vbo() {
    assert(_initialized);
    return new GLVertexBuffer(&_gl);
}

VertexArray *GLRenderer::make_vao() {
    assert(_initialized);
    return new GLVertexArray(&_gl);
}

ElementBuffer<unsigned int> *GLRenderer::make_int_ebo() {
    assert(_initialized);
    return new GLElementBuffer<unsigned int>(&_gl);
}

ElementBuffer<unsigned short> *GLRenderer::make_short_ebo() {
    assert(_initialized);
    return new GLElementBuffer<unsigned short>(&_gl);
}

ElementBuffer<unsigned char> *GLRenderer::make_char_ebo() {
    assert(_initialized);
    return new GLElementBuffer<unsigned char>(&_gl);
}

Material *GLRenderer::make_material() {
    assert(_initialized);
    return new GLMaterial(&_gl);
}

Shader *GLRenderer::make_shader() {
    assert(_initialized);
    return new GLShader(&_gl);
}

Texture *GLRenderer::make_texture(TextureInitOptions options) {
    assert(_initialized);
    return new GLTexture(&_gl, options);
}

FrameBufferSurface *GLRenderer::make_framebuffer() {
    assert(_initialized);
    return new GLFrameBufferSurface(&_gl);
}

RenderBuffer *GLRenderer::make_renderbuffer() {
    assert(_initialized);
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
