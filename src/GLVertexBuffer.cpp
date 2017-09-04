#include <algorithm>
#include "giygas_internal/GLVertexBuffer.hpp"

using namespace giygas;

GLVertexBuffer::GLVertexBuffer(GL *gl) {
    _gl = gl;
    _data = nullptr;
    _length = 0;
    _channel_count = 0;
    gl->gen_buffers(1, &_handle);
    gl->gen_vertex_arrays(1, &_vertex_array);
}

GLVertexBuffer::GLVertexBuffer(GLVertexBuffer &&other) noexcept {
    *this = std::move(other);
}

GLVertexBuffer &GLVertexBuffer::operator=(GLVertexBuffer &&other) noexcept {
    _gl = other._gl;
    _data = other._data;
    _length = other._length;
    _handle = other._handle;
    _vertex_array = other._vertex_array;
    _channel_count = other._channel_count;
    other._data = nullptr;
    other._handle = 0;
    other._vertex_array = 0;

    return *this;
}

GLVertexBuffer::~GLVertexBuffer() {
    delete[] _data;
    _gl->delete_buffers(1, &_handle);
    _gl->delete_vertex_arrays(1, &_vertex_array);
}

void GLVertexBuffer::set_layout(const VertexBufferLayout &layout) {
    const auto& channels = layout.get_channels();

    int stride = 0;
    for (int size : channels) {
        stride += size;
    }

    int offset = 0;
    GLuint index = 0;

    _gl->bind_vertex_array(_vertex_array);
    _gl->bind_buffer(GL_ARRAY_BUFFER, _handle);

    for (int size : channels) {
        _gl->vertex_attrib_pointer(
            index,                             // index
            size,                              // size
            GL_FLOAT,                          // type
            GL_FALSE,                          // normalized
            stride - size,                     // stride
            reinterpret_cast<GLvoid *>(offset) // pointer
        );

        _gl->enable_vertex_attrib_array(index);

        offset += size;
        ++index;
    }

    _channel_count = static_cast<GLuint>(index);
}

bool GLVertexBuffer::has_layout() const {
    return _channel_count > 0;
}

void GLVertexBuffer::set_vector2_channel(
    int offset, const Vector2* verts, int count
) {
    set_data(
        offset,
        reinterpret_cast<const float *>(verts),
        count * 2
    );
}

void GLVertexBuffer::set_vector3_channel(
    int offset, const Vector3* verts, int count
) {
    set_data(
        offset,
        reinterpret_cast<const float *>(verts),
        count * 3
    );
}

void GLVertexBuffer::set_vector4_channel(
    int offset, const Vector4* verts, int count
) {
    set_data(
        offset,
        reinterpret_cast<const float *>(verts),
        count * 4
    );
}

GLuint GLVertexBuffer::get_handle() const {
    return _handle;
}

void GLVertexBuffer::set_data(int offset, const float *data, int count) {
    int required_length = offset + count;
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
            _length * sizeof(GLfloat),
            _data,
            GL_DYNAMIC_DRAW
        );
    }
    else {
        _gl->buffer_sub_data(
            GL_ARRAY_BUFFER,
            offset * sizeof(GLfloat),
            count * sizeof(GLfloat),
            data
        );
    }
}