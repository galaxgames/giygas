#include <algorithm>
#include "GLVertexBuffer.hpp"
#include "operations/DeleteBufferGLOperation.hpp"
#include "operations/BufferDataGLOperation.hpp"
#include "operations/BufferSubDataGLOperation.hpp"

using namespace giygas;

GLVertexBuffer::GLVertexBuffer(GLRenderer *renderer) {
    _renderer = renderer;
    _data = nullptr;
    _size = 0;
    _channel_count = 0;

    GenBufferGLOperation gen_op;
    _renderer->add_operation(&gen_op, nullptr);
    _handle = gen_op.get_generated_handle();
}

GLVertexBuffer::GLVertexBuffer(GLVertexBuffer &&other) noexcept {
    *this = std::move(other);
}

GLVertexBuffer &GLVertexBuffer::operator=(GLVertexBuffer &&other) noexcept {
    _renderer = other._renderer;
    _data = other._data;
    _size = other._size;
    _channel_count = other._channel_count;
    _handle = other._handle;
    other._data = nullptr;
    other._handle = 0;
    return *this;
}

GLVertexBuffer::~GLVertexBuffer() {
    delete[] _data;
    Pool<DeleteBufferGLOperation> &pool = _renderer->pools().delete_buffer_ops;
    DeleteBufferGLOperation *delete_op = pool.take();
    delete_op->set_handle(_handle);
    _renderer->add_operation(delete_op, &pool);
}

RendererType GLVertexBuffer::get_renderer_type() const {
    return RendererType::OpenGL;
}

GLuint GLVertexBuffer::get_handle() const {
    return _handle;
}

void GLVertexBuffer::set_data(size_t offset, const void *data, size_t count) {
    const byte *bdata = reinterpret_cast<const byte *>(data);
    size_t required_length = offset + count;
    bool need_new_buffer = required_length > _size;
    if (need_new_buffer) {
        auto *new_data = new byte[required_length];
        std::copy_n(_data, _size, new_data);
        _data = new_data;
        _size = required_length;
    }

    std::copy_n(bdata, count, _data + offset);

    if (need_new_buffer) {
        Pool<BufferDataGLOperation> &pool = _renderer->pools().buffer_data_ops;
        BufferDataGLOperation *op = pool.take();
        op->set(
            _handle,
            GL_ARRAY_BUFFER,
            static_cast<GLsizei>(_size),
            _data
        );
        _renderer->add_operation(op, &pool);
    }
    else {
        Pool<BufferSubDataGLOperation> &pool
            = _renderer->pools().buffer_sub_data_ops;
        BufferSubDataGLOperation *op = pool.take();
        op->set(
            _handle,
            GL_ARRAY_BUFFER,
            static_cast<GLsizei>(offset),
            static_cast<GLsizei>(count),
            data
        );
        _renderer->add_operation(op, &pool);
    }
}
