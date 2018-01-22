#include "GLFrameBufferSurface.hpp"
#include "GLRenderBuffer.hpp"
#include "GLTexture.hpp"
#include "GLRenderer.hpp"
#include "operations/GenFramebufferGLOperation.hpp"
#include <cassert>

using namespace giygas;
using namespace std;

GLFrameBufferSurface::GLFrameBufferSurface(GLRenderer *renderer) {
    _renderer = renderer;
    GenFramebufferGLOperation gen_op;
    renderer->add_operation_and_notify(&gen_op, nullptr);
    _handle = gen_op.get_generated_handle();
}

GLFrameBufferSurface::GLFrameBufferSurface(
    GLFrameBufferSurface &&other
) noexcept
    : _surface_renderer(move(other._surface_renderer))
{
    move_common(move(other));
}

GLFrameBufferSurface &GLFrameBufferSurface::operator=(
    GLFrameBufferSurface &&other
) noexcept {
    _surface_renderer = move(other._surface_renderer);
    move_common(move(other));
    return *this;
}

void GLFrameBufferSurface::move_common(GLFrameBufferSurface &&other) noexcept {
    _renderer = other._renderer;
    _handle = other._handle;
    _width = other._width;
    _height = other._height;
    other._handle = 0;
}

GLFrameBufferSurface::~GLFrameBufferSurface() {
    Pool<DeleteFramebufferGLOperation> &pool
        = _renderer->pools().delete_framebuffer_ops;
    DeleteFramebufferGLOperation *delete_op = pool.take();
    delete_op->set_handle(_handle);
    _renderer->add_operation(delete_op, &pool);
}

void GLFrameBufferSurface::attach_texture(
    Texture *texture,
    SurfaceBufferType attachment_type
) {
    assert(texture->renderer_type() == RendererType::OpenGL);
    GLTexture *gl_texture = reinterpret_cast<GLTexture *>(texture);

    Pool<AttachTextureToFramebufferGLOperation> &pool
        = _renderer->pools().attach_texture_to_framebuffer_ops;
    AttachTextureToFramebufferGLOperation *op = pool.take();
    op->set(
        _handle,
        gl_texture->handle(),
        0,
        get_attachment_flag(attachment_type)
    );
    _renderer->add_operation(op, &pool);
}

void GLFrameBufferSurface::attach_renderbuffer(
    RenderBuffer *renderbuffer,
    SurfaceBufferType attachment_type
) {
    assert(renderbuffer->renderer_type() == RendererType::OpenGL);
    auto *gl_buffer = reinterpret_cast<GLRenderBuffer *>(renderbuffer);

    Pool<AttachRenderbufferToFramebufferGLOperation> &pool
        = _renderer->pools().attach_renderbuffer_to_framebuffer_ops;
    AttachRenderbufferToFramebufferGLOperation *op = pool.take();
    op->set(
        _handle,
        gl_buffer->handle(),
        get_attachment_flag(attachment_type)
    );
    _renderer->add_operation(op, &pool);
}

void GLFrameBufferSurface::set_size(unsigned int width, unsigned int height) {
    _width = width;
    _height = height;
}

unsigned int GLFrameBufferSurface::width() const {
    return _width;
}

unsigned int GLFrameBufferSurface::height() const {
    return _height;
}

void GLFrameBufferSurface::set_viewport(
    unsigned int x,
    unsigned int y,
    unsigned int width,
    unsigned int height
) {
    _surface_renderer.set_viewport(x, y, width, height);
}

void GLFrameBufferSurface::set_clear_color(Vector4 color) {
    _surface_renderer.set_clear_color(color);
}

void GLFrameBufferSurface::set_clear_depth(double value) {
    _surface_renderer.set_clear_depth(value);
}

void GLFrameBufferSurface::set_clear_stencil(int value) {
    _surface_renderer.set_clear_stencil(value);
}

void GLFrameBufferSurface::clear(SurfaceBufferType buffers) {
    _surface_renderer.clear(_renderer, _handle, buffers);
}

void GLFrameBufferSurface::draw(
    VertexArray *vao,
    ElementBuffer<unsigned int> *ebo,
    Material *material,
    ElementDrawInfo element_info,
    const PipelineOptions &pipeline
) {
    _surface_renderer.draw(_renderer, _handle, vao, ebo, material, element_info, pipeline);
}

void GLFrameBufferSurface::draw(
    VertexArray *vao,
    ElementBuffer<unsigned short> *ebo,
    Material *material,
    ElementDrawInfo element_info,
    const PipelineOptions &pipeline
) {
    _surface_renderer.draw(_renderer, _handle, vao, ebo, material, element_info, pipeline);
}

void GLFrameBufferSurface::draw(
    VertexArray *vao,
    ElementBuffer<unsigned char> *ebo,
    Material *material,
    ElementDrawInfo element_info,
    const PipelineOptions &pipeline
) {
    _surface_renderer.draw(_renderer, _handle, vao, ebo, material, element_info, pipeline);
}

GLenum GLFrameBufferSurface::get_attachment_flag(
    SurfaceBufferType attachment_type
) const {
  switch (attachment_type) {
  case SurfaceBufferType::Color:
      return GL_COLOR_ATTACHMENT0;
  case SurfaceBufferType::Depth:
      return GL_DEPTH_ATTACHMENT;
  case SurfaceBufferType::Stencil:
      return GL_STENCIL_ATTACHMENT;
  }
}
