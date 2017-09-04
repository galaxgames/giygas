#include <algorithm>
#include <glad/glad.h>
#include "giygas_internal/GLElementBuffer.hpp"

using namespace giygas;

GLElementBuffer::GLElementBuffer(GL *gl) {
    _gl = gl;
    _data = nullptr;
    _length = 0;
    gl->gen_buffers(1, &_handle);
}

GLElementBuffer::GLElementBuffer(GLElementBuffer &&other) noexcept {
    *this = std::move(other);
}

GLElementBuffer& GLElementBuffer::operator=(GLElementBuffer &&other) noexcept {
    _gl = other._gl;
    _handle = other._handle;
    _data = other._data;
    _length = other._length;
    other._handle = 0;
    other._data = nullptr;

    return *this;
}

GLElementBuffer::~GLElementBuffer() {
    delete[] _data;
    _gl->delete_buffers(1, &_handle);
}

void GLElementBuffer::set(int index, const unsigned int *elements, int count) {
    int required_length = index + count;
    bool need_new_buffer = required_length > _length;
    if (need_new_buffer) {
        GLuint *new_data = new GLuint[required_length];
        std::copy_n(_data, _length, new_data);
        _data = new_data;
        _length = required_length;
    }

    std::copy_n(elements, count, _data + index);
    _gl->bind_buffer(GL_ELEMENT_ARRAY_BUFFER, _handle);
    
    if (need_new_buffer) {
        _gl->buffer_data(
            GL_ELEMENT_ARRAY_BUFFER,
            _length * sizeof(GLuint),
            _data,
            GL_DYNAMIC_DRAW
        );
    }
    else {
        _gl->buffer_sub_data(
            GL_ELEMENT_ARRAY_BUFFER,
            _length * sizeof(GLuint),
            count * sizeof(GLuint),
            elements
        );
    }

}

GLuint GLElementBuffer::get_handle() const {
    return _handle;
}

GLuint GLElementBuffer::get_length() const {
    return _length;
}