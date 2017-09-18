#include <cassert>
#include <giygas_internal/GLVertexArray.hpp>
#include <glad/glad.h>
#include <giygas/GLRenderer.hpp>
#include <giygas_internal/GLVertexBuffer.hpp>
#include <giygas_internal/GLElementBuffer.hpp>
#include <giygas_internal/GLMaterial.hpp>
#include <giygas_internal/GLTexture.hpp>

using namespace std;
using namespace giygas;

GLRenderer::GLRenderer(shared_ptr<Window> window) :
    _window(move(window))
{
    gladLoadGL();
}

GLRenderer::GLRenderer(GLRenderer &&other) noexcept :
    _gl(move(other._gl)),
    _window(move(other._window))
{
}

GLRenderer& GLRenderer::operator=(GLRenderer &&other) noexcept {
    _gl = move(other._gl);
    _window = move(other._window);
    return *this;
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

void GLRenderer::set_clear_color(Vector4 color) {
    glClearColor(color.x, color.y, color.z, color.w);
}

void GLRenderer::set_clear_depth(double value) {
    glClearDepth(value);
}

void GLRenderer::set_clear_stencil(int value) {
    glClearStencil(value);
}

void GLRenderer::clear(SurfaceType surfaces) {
    glClear(get_clear_flags(surfaces));
}

#define DEFINE_DRAW_FUNCTION(type, gltype) \
void GLRenderer::draw( \
    VertexArray *vao, ElementBuffer<type> *ebo, Material *material, \
    ElementDrawInfo element_info \
) { \
    assert(ebo->get_renderer_type() == RendererType::OpenGL); \
    /* NOTE: Cannot directly reinterpret_cast GenericGLElementBuffer, */ \
    /* doing so will omit the pointer fixup needed for multiple inheritance */ \
    /* to work (thunking) */ \
    auto *gl_ebo = reinterpret_cast<GLElementBuffer<type> *>(ebo); \
    draw_internal(vao, gl_ebo, material, element_info, gltype); \
}

DEFINE_DRAW_FUNCTION(unsigned int, GL_UNSIGNED_INT);
DEFINE_DRAW_FUNCTION(unsigned short, GL_UNSIGNED_SHORT);
DEFINE_DRAW_FUNCTION(unsigned char, GL_UNSIGNED_BYTE);

void GLRenderer::draw_internal(
    VertexArray *vao, GenericGLElementBuffer *ebo, Material *material,
    ElementDrawInfo element_info, GLenum element_type
) {
    assert(vao->get_renderer_type() == RendererType::OpenGL);
    assert(material->renderer_type() == RendererType::OpenGL);

    auto *gl_vao = reinterpret_cast<GLVertexArray *>(vao);
    auto *gl_material = reinterpret_cast<GLMaterial *>(material);

    if (!gl_material->is_valid()) {
        //TODO: Log draw call aborted because of invalid shader.
        return;
    }

    _gl.bind_vertex_array(gl_vao->get_handle());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->handle());
    glUseProgram(gl_material->get_program());

    for (size_t i = 0, ilen = gl_material->get_texture_count(); i < ilen; ++i) {
        if (auto texture = gl_material->get_texture(i).lock()) {
            assert(texture->renderer_type() == RendererType::OpenGL);
            auto *gl_texture = reinterpret_cast<GLTexture *>(texture.get());
            glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + i));
            glBindTexture(GL_TEXTURE_2D, gl_texture->get_handle());
        }
    }

    glDrawElements(
        get_gl_primitive(Primitive::Triangles),
        static_cast<GLsizei>(element_info.count),
        element_type,
        (void *)element_info.offset
    );
}

GLenum GLRenderer::get_gl_primitive(Primitive primitive) {
    switch (primitive) {
        case Primitive::Points:
            return GL_POINTS;
        case Primitive::LineStrip:
            return GL_LINE_STRIP;
        case Primitive::LineLoop:
            return GL_LINE_LOOP;
        case Primitive::Lines:
            return GL_LINES;
        case Primitive::TriangleStrip:
            return GL_TRIANGLE_STRIP;
        case Primitive::TriangleFan:
            return GL_TRIANGLE_FAN;
        case Primitive::Triangles:
            return GL_TRIANGLES;
    }
}

GLenum GLRenderer::get_clear_flags(SurfaceType surfaces) {
    GLenum value = GL_NONE;

    if (has_flag(surfaces, SurfaceType::Color)) {
        value |= GL_COLOR_BUFFER_BIT;
    }
    if (has_flag(surfaces, SurfaceType::Depth)) {
        value |= GL_DEPTH_BUFFER_BIT;
    }
    if (has_flag(surfaces, SurfaceType::Stencil)) {
        value |= GL_STENCIL_BUFFER_BIT;
    }
    return value;
}
