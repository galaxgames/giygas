#include <algorithm>
#include <glad/glad.h>
#include "GLElementBuffer.hpp"

using namespace giygas;

GLElementBuffer::GLElementBuffer() {
    _data = nullptr;
    _length = 0;
    glGenBuffers(1, &_handle);
}

GLElementBuffer::GLElementBuffer(GLElementBuffer &&other) noexcept {
    _handle = other._handle;
    _data = other._data;
    _length = other._length;
    other._handle = 0;
    other._data = nullptr;
}

GLElementBuffer::~GLElementBuffer() {
    delete[] _data;
    glDeleteBuffers(1, &_handle);
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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _handle);
    
    if (need_new_buffer) {
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            _length * sizeof(GLuint),
            _data,
            GL_DYNAMIC_DRAW
        );
    }
    else {
        glBufferSubData(
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