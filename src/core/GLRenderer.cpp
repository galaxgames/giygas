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

    #ifndef NDEBUG
        _initialized = true;
    #endif

    set_polygon_culling_enabled(options.polygon_culling.enabled);
    set_polygon_culling_mode(options.polygon_culling.mode);
    set_front_face_winding(options.polygon_culling.front_face_vertex_winding);

    set_depth_test_enabled(options.depth_buffer.depth_test_enabled);
    set_depth_mask_enabled(options.depth_buffer.mask_enabled);
    set_depth_function(options.depth_buffer.function);
    set_depth_range(
        options.depth_buffer.range_near,
        options.depth_buffer.range_far
    );

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
    assert(_initialized);
    glCullFace(value == PolygonCullingMode::BackFace ? GL_BACK : GL_FRONT);
}

void GLRenderer::set_front_face_winding(VertexWinding value) {
    assert(_initialized);
    glFrontFace(value == VertexWinding::CounterClockwise ? GL_CCW : GL_CW);
}

void GLRenderer::set_depth_test_enabled(bool value) {
    assert(_initialized);
    if (value) {
        glEnable(GL_DEPTH_TEST);
    }
    else {
        glDisable(GL_DEPTH_TEST);
    }
}

void GLRenderer::set_depth_mask_enabled(bool value) {
    assert(_initialized);
    glDepthMask(static_cast<GLboolean>(value));
}

void GLRenderer::set_depth_function(DepthFunction value) {
    assert(_initialized);
    glDepthFunc(depth_function_to_enum(value));
}

void GLRenderer::set_depth_range(double near, double far) {
    assert(_initialized);
    glDepthRange(near, far);
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

GLenum GLRenderer::depth_function_to_enum(DepthFunction function) {
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
