#include <giygas_internal/GLRenderBuffer.hpp>
#include <giygas_internal/gl_enum_conversions.hpp>
#include <utility>

using namespace giygas;
using namespace std;

GLRenderBuffer::GLRenderBuffer(GL *gl) {
    _gl = gl;
    _gl->gen_renderbuffers(1, &_handle);
}

GLRenderBuffer::GLRenderBuffer(GLRenderBuffer &&other) noexcept {
    *this = move(other);
}

GLRenderBuffer &GLRenderBuffer::operator=(GLRenderBuffer &&other) noexcept {
    _gl = other._gl;
    _handle = other._handle;
    other._handle = 0;
    return *this;
}

GLRenderBuffer::~GLRenderBuffer() {
    _gl->delete_renderbuffers(1, &_handle);
}

RendererType GLRenderBuffer::renderer_type() const {
    return RendererType::OpenGL;
}

void GLRenderBuffer::create_storage(
    unsigned int width,
    unsigned int height,
    TextureFormat format
) {
    _gl->bind_renderbuffer(GL_RENDERBUFFER, _handle);
    _gl->renderbuffer_storage(
        GL_RENDERBUFFER,
        get_gl_texture_format(format),
        static_cast<GLsizei>(width),
        static_cast<GLsizei>(height)
    );
}

GLuint GLRenderBuffer::handle() const {
    return _handle;
}
