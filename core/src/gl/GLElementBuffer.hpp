#pragma once
#include <giygas/IndexBuffer.hpp>
#include "GL.hpp"

namespace giygas {

    class GLRenderer;

    class GenericGLElementBuffer {
    public:
        virtual ~GenericGLElementBuffer() = default;
        virtual GLuint handle() const = 0;
    };

    template <typename T>
    class GLElementBuffer : public IndexBuffer<T>,
        public GenericGLElementBuffer
    {
        GLRenderer *_renderer;
        GLuint _handle;
        T *_data;
        size_t _count;

#ifndef NDEBUG
        T _max_element;

        void update_max_element();
#endif

    public:
        GLElementBuffer(GLRenderer *renderer);
        GLElementBuffer(const GLElementBuffer &) = delete;
        GLElementBuffer(GLElementBuffer &&) noexcept;
        GLElementBuffer &operator=(const GLElementBuffer &) = delete;
        GLElementBuffer &operator=(GLElementBuffer &&) noexcept;
        virtual ~GLElementBuffer();
        RendererType get_renderer_type() const override;
        void set(size_t index, const T *elements, size_t count) override;
        size_t count() const override;

        GLuint handle() const override;
    };

    extern template class GLElementBuffer<uint32_t>;
    extern template class GLElementBuffer<uint16_t>;
    extern template class GLElementBuffer<uint8_t>;
}
