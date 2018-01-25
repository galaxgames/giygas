#include "GLSurfaceRenderer.hpp"
#include "GLVertexArray.hpp"
#include "GLMaterial.hpp"
#include "GLTexture.hpp"
#include "operations/RenderGLOperation.hpp"
#include "Pool.hpp"
#include "GLRenderer.hpp"
#include <cassert>

using namespace giygas;

GLSurfaceRenderer::GLSurfaceRenderer()
    : _clear_color(0, 0, 0, 0)
    , _clear_depth(0)
    , _clear_stencil(0)
    , _viewport_x(0)
    , _viewport_y(0)
    , _viewport_width(0)
    , _viewport_height(0)
{}

void GLSurfaceRenderer::set_viewport(
    unsigned int x,
    unsigned int y,
    unsigned int width,
    unsigned int height
) {
    _viewport_x = x;
    _viewport_y = y;
    _viewport_width = width;
    _viewport_height = height;
}

void GLSurfaceRenderer::set_clear_color(Vector4 color) {
    _clear_color = color;
}

void GLSurfaceRenderer::set_clear_depth(double value) {
    _clear_depth = value;
}

void GLSurfaceRenderer::set_clear_stencil(int value) {
    _clear_stencil = value;
}

void GLSurfaceRenderer::clear(
    GLRenderer *renderer,
    GLuint framebuffer,
    SurfaceBufferType surfaces
) {
    Pool<ClearGLOperation> &pool = renderer->pools().clear_ops;
    ClearGLOperation *op = pool.take();
    op->set(framebuffer, get_clear_flags(surfaces));
    op->set_color(_clear_color.x, _clear_color.y, _clear_color.z, _clear_color.w);
    op->set_depth(_clear_depth);
    op->set_stencil(_clear_stencil);
    renderer->add_operation(op, &pool);
}


#define DEFINE_DRAW_FUNCTION(type, gltype) \
void GLSurfaceRenderer::draw( \
    GLRenderer *renderer, \
    GLuint framebuffer, \
    VertexArray *vao, \
    ElementBuffer<type> *ebo, \
    Material *material, \
    ElementDrawInfo element_info \
) { \
    assert(ebo->get_renderer_type() == RendererType::OpenGL); \
    /* NOTE: Cannot directly reinterpret_cast GenericGLElementBuffer,
     * doing so will omit the pointer fixup needed for multiple inheritance
     * to work (thunking)
     */ \
    auto *gl_ebo = reinterpret_cast<GLElementBuffer<type> *>(ebo); \
    draw_internal( \
        renderer, \
        framebuffer, \
        vao, \
        gl_ebo, \
        material, \
        element_info, \
        gltype); \
}

DEFINE_DRAW_FUNCTION(unsigned int, GL_UNSIGNED_INT);
DEFINE_DRAW_FUNCTION(unsigned short, GL_UNSIGNED_SHORT);
DEFINE_DRAW_FUNCTION(unsigned char, GL_UNSIGNED_BYTE);

void GLSurfaceRenderer::draw_internal(
    GLRenderer *renderer,
    GLuint framebuffer,
    VertexArray *vao,
    GenericGLElementBuffer *ebo,
    Material *material,
    ElementDrawInfo element_info,
    GLenum element_type
) {
    assert(vao->get_renderer_type() == RendererType::OpenGL);
    assert(material->renderer_type() == RendererType::OpenGL);

    auto *gl_vao = reinterpret_cast<GLVertexArray *>(vao);
    auto *gl_material = reinterpret_cast<GLMaterial *>(material);

    assert(gl_material->is_valid());

    // TODO: debug element buffer validation

//    Pool<SetPipelineGLOperation> &pipeline_pool
//        = renderer->pools()._set_pipeline_ops;
//    SetPipelineGLOperation *pipeline_op = pipeline_pool.take();
//    pipeline_op->set(pipeline);
//    renderer->add_operation(pipeline_op, &pipeline_pool);

    Pool<RenderGLOperation> &pool = renderer->pools().render_ops;
    RenderGLOperation *render_op = pool.take();

    render_op->set(
        framebuffer,
        gl_vao->get_handle(),
        ebo->handle(),
        gl_material->get_program()
    );

    render_op->set_viewport(
        static_cast<GLint>(_viewport_x),
        static_cast<GLint>(_viewport_y),
        static_cast<GLsizei>(_viewport_width),
        static_cast<GLsizei>(_viewport_height)
    );

    render_op->clear_textures();
    for (size_t i = 0, ilen = gl_material->get_texture_count(); i < ilen; ++i) {
        if (auto texture = gl_material->get_texture(i).lock()) {
            assert(texture->renderer_type() == RendererType::OpenGL);
            auto *gl_texture = reinterpret_cast<GLTexture *>(texture.get());
            render_op->add_texture(gl_texture->handle());
        }
        else {
            render_op->add_texture(0);
        }
    }

    render_op->set_primitive(get_gl_primitive(Primitive::Triangles));
    render_op->set_element_type(element_type);
    render_op->set_draw_info(element_info);

    renderer->add_operation(render_op, &pool);
}

GLenum GLSurfaceRenderer::get_gl_primitive(Primitive primitive) {
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

GLenum GLSurfaceRenderer::get_clear_flags(SurfaceBufferType surfaces) {
    GLenum value = GL_NONE;

    if (has_flag(surfaces, SurfaceBufferType::Color)) {
        value |= GL_COLOR_BUFFER_BIT;
    }
    if (has_flag(surfaces, SurfaceBufferType::Depth)) {
        value |= GL_DEPTH_BUFFER_BIT;
    }
    if (has_flag(surfaces, SurfaceBufferType::Stencil)) {
        value |= GL_STENCIL_BUFFER_BIT;
    }
    return value;
}
