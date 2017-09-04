#include <glad/glad.h>
#include "giygas/GLRenderer.hpp"
#include "giygas_internal/GLVertexBuffer.hpp"
#include "giygas_internal/GLElementBuffer.hpp"
#include "giygas_internal/GLMaterial.hpp"
#include "giygas_internal/GLTexture.hpp"

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

VertexBuffer* GLRenderer::make_vbo() {
    return new GLVertexBuffer(&_gl);
}

ElementBuffer* GLRenderer::make_ebo() {
    return new GLElementBuffer(&_gl);
}

Material* GLRenderer::make_material() {
    return new GLMaterial(&_gl);
}

Shader* GLRenderer::make_shader() {
    return new GLShader(&_gl);
}

void GLRenderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderer::draw(
    VertexBuffer *vbo, ElementBuffer *ebo, Material *material
) {
    // TODO: Type checking
    GLVertexBuffer *gl_vbo = reinterpret_cast<GLVertexBuffer *>(vbo);
    GLElementBuffer *gl_ebo = reinterpret_cast<GLElementBuffer *>(ebo);
    GLMaterial *gl_material = reinterpret_cast<GLMaterial *>(material);

    if (!gl_material->is_valid()) {
        //TODO: Log draw call aborted because of invalid shader.
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, gl_vbo->get_handle());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_ebo->get_handle());
    glUseProgram(gl_material->get_program());

    for (int i = 0, ilen = gl_material->get_texture_count(); i < ilen; ++i) {
        if (auto texture = gl_material->get_texture(i).lock()) {
            // TODO: typechecking
            auto *gl_texture = reinterpret_cast<GLTexture *>(texture.get());
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, gl_texture->get_handle());
        }
    }

    glDrawElements(
        get_gl_primitive(Primitive::Triangles),
        gl_ebo->get_length(),
        GL_UNSIGNED_INT,
        nullptr
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