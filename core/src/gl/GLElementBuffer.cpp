#include <algorithm>
#include <glad/glad.h>
#include "GLElementBuffer.hpp"
#include "GLRenderer.hpp"
#include "operations/GenBufferGLOperation.hpp"
#include "operations/DeleteBufferGLOperation.hpp"
#include "operations/BufferDataGLOperation.hpp"
#include "operations/BufferSubDataGLOperation.hpp"

using namespace giygas;

template <typename T>
GLElementBuffer<T>::GLElementBuffer(GLRenderer *renderer) {
    _renderer = renderer;
    _data = nullptr;
    _count = 0;

    GenBufferGLOperation gen_op;
    _renderer->add_operation_and_notify(&gen_op, nullptr);
    _handle = gen_op.get_generated_handle();
}

template <typename T>
GLElementBuffer<T>::GLElementBuffer(GLElementBuffer &&other) noexcept {
    *this = std::move(other);
}

template <typename T>
GLElementBuffer<T>& GLElementBuffer<T>::operator=(GLElementBuffer &&other) noexcept {
    _renderer = other._renderer;
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
    Pool<DeleteBufferGLOperation> &pool = _renderer->pools().delete_buffer_ops;
    DeleteBufferGLOperation *delete_op = pool.take();
    delete_op->set_handle(_handle);
    _renderer->add_operation(delete_op, &pool);
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
    //_gl->bind_buffer(GL_ELEMENT_ARRAY_BUFFER, _handle);

    GLOperationPools &pools = _renderer->pools();

    if (need_new_buffer) {
        Pool<BufferDataGLOperation> &pool = pools.buffer_data_ops;
        BufferDataGLOperation *op = pool.take();
        op->set(
            _handle,
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizei>(_count * sizeof(T)),
            _data
        );
        _renderer->add_operation(op, &pool);
    }
    else {
        Pool<BufferSubDataGLOperation> &pool = pools.buffer_sub_data_ops;
        BufferSubDataGLOperation *op = pool.take();
        op->set(
            _handle,
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizei>(_count * sizeof(T)),
            static_cast<GLsizei>(count * sizeof(T)),
            elements
        );
        _renderer->add_operation(op, &pool);
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

