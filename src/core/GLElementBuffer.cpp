#include <algorithm>
#include <glad/glad.h>
#include "giygas_internal/GLElementBuffer.hpp"

using namespace giygas;

template <typename T>
GLElementBuffer<T>::GLElementBuffer(GL *gl) {
    _gl = gl;
    _data = nullptr;
    _count = 0;
    gl->gen_buffers(1, &_handle);
}

template <typename T>
GLElementBuffer<T>::GLElementBuffer(GLElementBuffer &&other) noexcept {
    *this = std::move(other);
}

template <typename T>
GLElementBuffer<T>& GLElementBuffer<T>::operator=(GLElementBuffer &&other) noexcept {
    _gl = other._gl;
    _handle = other._handle;
    _data = other._data;
    _count = other._count;
    other._handle = 0;
    other._data = nullptr;

    return *this;
}

template <typename T>
GLElementBuffer<T>::~GLElementBuffer() {
    delete[] _data;
    _gl->delete_buffers(1, &_handle);
}

template <typename T>
RendererType GLElementBuffer<T>::get_renderer_type() const {
    return RendererType::OpenGL;
}

template <typename T>
void GLElementBuffer<T>::set(size_t index, const T *elements, size_t count) {
    size_t required_length = index + count;
    bool need_new_buffer = required_length > _count;
    if (need_new_buffer) {
        T *new_data = new T[required_length];
        std::copy_n(_data, _count, new_data);
        _data = new_data;
        _count = required_length;
    }

    std::copy_n(elements, count, _data + index);
    _gl->bind_buffer(GL_ELEMENT_ARRAY_BUFFER, _handle);
    
    if (need_new_buffer) {
        _gl->buffer_data(
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizei>(_count * sizeof(T)),
            _data,
            GL_DYNAMIC_DRAW
        );
    }
    else {
        _gl->buffer_sub_data(
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizei>(_count * sizeof(T)),
            static_cast<GLsizei>(count * sizeof(T)),
            elements
        );
    }

}

template <typename T>
GLuint GLElementBuffer<T>::handle() const {
    return _handle;
}

template <typename T>
size_t GLElementBuffer<T>::count() const {
    return _count;
}

namespace giygas {
    template class GLElementBuffer<unsigned int>;
    template class GLElementBuffer<unsigned short>;
    template class GLElementBuffer<unsigned char>;
}

