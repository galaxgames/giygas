#include <algorithm>
#include "giygas_internal/GLVertexBuffer.hpp"

using namespace giygas;

GLVertexBuffer::GLVertexBuffer(GL *gl) {
    _gl = gl;
    _data = nullptr;
    _length = 0;
    _channel_count = 0;
    gl->gen_buffers(1, &_handle);
}

GLVertexBuffer::GLVertexBuffer(GLVertexBuffer &&other) noexcept {
    *this = std::move(other);
}

GLVertexBuffer &GLVertexBuffer::operator=(GLVertexBuffer &&other) noexcept {
    _gl = other._gl;
    _data = other._data;
    _length = other._length;
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

void GLVertexBuffer::set_data(size_t offset, const float *data, size_t count) {
    size_t required_length = offset + count;
    bool need_new_buffer = required_length > _length;
    if (need_new_buffer) {
        auto *new_data = new float[required_length];
        std::copy_n(_data, _length, new_data);
        _data = new_data;
        _length = required_length;
    }

    std::copy_n(data, count, _data + offset);
    _gl->bind_buffer(GL_ARRAY_BUFFER, _handle);

    if (need_new_buffer) {
        _gl->buffer_data(
            GL_ARRAY_BUFFER,
            static_cast<GLsizei>(_length * sizeof(GLfloat)),
            _data,
            GL_DYNAMIC_DRAW
        );
    }
    else {
        _gl->buffer_sub_data(
            GL_ARRAY_BUFFER,
            static_cast<GLsizei>(offset * sizeof(GLfloat)),
            static_cast<GLsizei>(count * sizeof(GLfloat)),
            data
        );
    }
}

void GLVertexBuffer::set_data(size_t offset, const Vector4 *data, size_t count) {
    set_data(offset, reinterpret_cast<const float *>(data), count * 4);
}
