#include <giygas_internal/GLFrameBufferSurface.hpp>
#include <giygas_internal/GLRenderBuffer.hpp>
#include <giygas_internal/GLTexture.hpp>
#include <cassert>

using namespace giygas;
using namespace std;

GLFrameBufferSurface::GLFrameBufferSurface(GL *gl) {
    _gl = gl;
    gl->gen_framebuffers(1, &_handle);
}

GLFrameBufferSurface::GLFrameBufferSurface(
    GLFrameBufferSurface &&other
) noexcept
    : _renderer(move(other._renderer))
{
    move_common(move(other));
}

GLFrameBufferSurface &GLFrameBufferSurface::operator=(
    GLFrameBufferSurface &&other
) noexcept {
    _renderer = move(other._renderer);
    move_common(move(other));
    return *this;
}

void GLFrameBufferSurface::move_common(GLFrameBufferSurface &&other) noexcept {
    _gl = other._gl;
    _handle = other._handle;
    _width = other._width;
    _height = other._height;
    other._handle = 0;
}

GLFrameBufferSurface::~GLFrameBufferSurface() {
    _gl->delete_framebuffers(1, &_handle);
}

void GLFrameBufferSurface::attach_texture(
    Texture *texture,
    SurfaceBufferType attachment_type
) {
    assert(texture->renderer_type() == RendererType::OpenGL);
    GLTexture *gl_texture = reinterpret_cast<GLTexture *>(texture);
    _gl->bind_framebuffer(GL_FRAMEBUFFER, _handle);
    _gl->framebuffer_texture_2d(
        GL_FRAMEBUFFER,
        get_attachment_flag(attachment_type),
        GL_TEXTURE_2D,
        gl_texture->handle(),
        0
    );
}

void GLFrameBufferSurface::attach_renderbuffer(
    RenderBuffer *renderbuffer,
    SurfaceBufferType attachment_type
) {
    assert(renderbuffer->renderer_type() == RendererType::OpenGL);
    auto *gl_buffer = reinterpret_cast<GLRenderBuffer *>(renderbuffer);
    _gl->bind_framebuffer(GL_FRAMEBUFFER, _handle);
    _gl->framebuffer_renderbuffer(
        GL_FRAMEBUFFER,
        get_attachment_flag(attachment_type),
        GL_RENDERBUFFER,
        gl_buffer->handle()
    );
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
    _renderer.set_viewport(x, y, width, height);
}

void GLFrameBufferSurface::set_clear_color(Vector4 color) {
    _renderer.set_clear_color(color);
}

void GLFrameBufferSurface::set_clear_depth(double value) {
    _renderer.set_clear_depth(value);
}

void GLFrameBufferSurface::set_clear_stencil(int value) {
    _renderer.set_clear_stencil(value);
}

void GLFrameBufferSurface::clear(SurfaceBufferType buffers) {
    _gl->bind_framebuffer(GL_FRAMEBUFFER, _handle);
    _renderer.clear(buffers);
}

void GLFrameBufferSurface::draw(
    VertexArray *vao,
    ElementBuffer<unsigned int> *ebo,
    Material *material,
    ElementDrawInfo element_info
) {
    _gl->bind_framebuffer(GL_FRAMEBUFFER, _handle);
    _renderer.draw(_gl, vao, ebo, material, element_info);
}

void GLFrameBufferSurface::draw(
    VertexArray *vao,
    ElementBuffer<unsigned short> *ebo,
    Material *material,
    ElementDrawInfo element_info
) {
    _gl->bind_framebuffer(GL_FRAMEBUFFER, _handle);
    _renderer.draw(_gl, vao, ebo, material, element_info);
}

void GLFrameBufferSurface::draw(
    VertexArray *vao,
    ElementBuffer<unsigned char> *ebo,
    Material *material,
    ElementDrawInfo element_info
) {
    _gl->bind_framebuffer(GL_FRAMEBUFFER, _handle);
    _renderer.draw(_gl, vao, ebo, material, element_info);
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
