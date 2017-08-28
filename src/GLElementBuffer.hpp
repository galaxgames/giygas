#pragma once
#include <giygas/ElementBuffer.hpp>

namespace giygas {
    class GLElementBuffer : public ElementBuffer {
        GLuint _handle;
        GLuint *_data;
        size_t _length;

    public:
        GLElementBuffer();
        GLElementBuffer(const GLElementBuffer &) = delete;
        GLElementBuffer& operator=(const GLElementBuffer &) = delete;
        GLElementBuffer(GLElementBuffer &&) noexcept;
        virtual ~GLElementBuffer();

        void set(int index, const unsigned int *elements, int count) override;

        GLuint get_handle() const;
        GLuint get_length() const;
    };
}