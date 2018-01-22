#include "GLRenderBuffer.hpp"
#include "gl_enum_conversions.hpp"
#include "GLRenderer.hpp"
#include "operations/GenRenderbufferGLOperation.hpp"
#include <utility>

using namespace giygas;
using namespace std;

GLRenderBuffer::GLRenderBuffer(GLRenderer *renderer) {
    _renderer = renderer;
    GenRenderbufferGLOperation gen_op;
    renderer->add_operation_and_notify(&gen_op, nullptr);
    _handle = gen_op.get_generated_handle();
}

GLRenderBuffer::GLRenderBuffer(GLRenderBuffer &&other) noexcept {
    *this = move(other);
}

GLRenderBuffer &GLRenderBuffer::operator=(GLRenderBuffer &&other) noexcept {
    _renderer = other._renderer;
    _handle = other._handle;
    other._handle = 0;
    return *this;
}

GLRenderBuffer::~GLRenderBuffer() {
    Pool<DeleteRenderbufferGLOperation> &pool
        = _renderer->pools().delete_renderbuffer_ops;
    DeleteRenderbufferGLOperation *del_op = pool.take();
    del_op->set(_handle);
    _renderer->add_operation(del_op, &pool);
}

RendererType GLRenderBuffer::renderer_type() const {
    return RendererType::OpenGL;
}

void GLRenderBuffer::create_storage(
    unsigned int width,
    unsigned int height,
    TextureFormat format
) {
    Pool<CreateRenderbufferStorageGLOperation> &pool
        = _renderer->pools().create_renderbuffer_storage_ops;
    CreateRenderbufferStorageGLOperation *op = pool.take();
    op->set(
        _handle,
        get_gl_texture_format(format),
        static_cast<GLsizei>(width),
        static_cast<GLsizei>(height)
    );
    _renderer->add_operation(op, &pool);
}

GLuint GLRenderBuffer::handle() const {
    return _handle;
}
