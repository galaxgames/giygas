#pragma once
#include <giygas/ElementBuffer.hpp>
#include <giygas/GL.hpp>

namespace giygas {
    class GLElementBuffer : public ElementBuffer {
        GL *_gl;
        GLuint _handle;
        GLuint *_data;
        size_t _length;

    public:
        GLElementBuffer(GL *gl);
        GLElementBuffer(const GLElementBuffer &) = delete;
        GLElementBuffer(GLElementBuffer &&) noexcept;
        GLElementBuffer &operator=(const GLElementBuffer &) = delete;
        GLElementBuffer &operator=(GLElementBuffer &&) noexcept;
        virtual ~GLElementBuffer();

        void set(int index, const unsigned int *elements, int count) override;

        GLuint get_handle() const;
        GLuint get_length() const;
    };
}