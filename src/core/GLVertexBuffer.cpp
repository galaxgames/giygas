#include <algorithm>
#include "giygas_internal/GLVertexBuffer.hpp"

using namespace giygas;

GLVertexBuffer::GLVertexBuffer(GL *gl) {
    _gl = gl;
    _data = nullptr;
    _size = 0;
    _channel_count = 0;
    gl->gen_buffers(1, &_handle);
}

GLVertexBuffer::GLVertexBuffer(GLVertexBuffer &&other) noexcept {
    *this = std::move(other);
}

GLVertexBuffer &GLVertexBuffer::operator=(GLVertexBuffer &&other) noexcept {
    _gl = other._gl;
    _data = other._data;
    _size = other._size;
    _handle = other._handle;
    _channel_count = other._channel_count;
    other._data = nullptr;
    other._handle = 0;

    return *this;
}

GLVertexBuffer::~GLVertexBuffer() {
    delete[] _data;
    _gl->delete_buffers(1, &_handle);
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
    _gl->bind_buffer(GL_ARRAY_BUFFER, _handle);

    if (need_new_buffer) {
        _gl->buffer_data(
            GL_ARRAY_BUFFER,
            static_cast<GLsizei>(_size),
            _data,
            GL_DYNAMIC_DRAW
        );
    }
    else {
        _gl->buffer_sub_data(
            GL_ARRAY_BUFFER,
            static_cast<GLsizei>(offset),
            static_cast<GLsizei>(count),
            data
        );
    }
}
