#pragma once
#include <giygas/ElementBuffer.hpp>
#include <giygas_internal/GL.hpp>

namespace giygas {

    class GenericGLElementBuffer {
    public:
        virtual ~GenericGLElementBuffer() = default;
        virtual GLuint handle() const = 0;
    };

    template <typename T>
    class GLElementBuffer : public ElementBuffer<T>,
        public GenericGLElementBuffer
    {
        GL *_gl;
        GLuint _handle;
        T *_data;
        size_t _count;

    public:
        GLElementBuffer(GL *gl);
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

    extern template class GLElementBuffer<unsigned int>;
    extern template class GLElementBuffer<unsigned short>;
    extern template class GLElementBuffer<unsigned char>;
}
