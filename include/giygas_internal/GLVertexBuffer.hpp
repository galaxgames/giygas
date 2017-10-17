#pragma once
#include <glad/glad.h>
#include <giygas_internal/GL.hpp>
#include <giygas/VertexBuffer.hpp>

namespace giygas {
    class GLVertexBuffer : public VertexBuffer {
        typedef unsigned char byte;

        GL *_gl;

        // handle to the GL buffer
        GLuint _handle;

        // The internal buffer
        byte *_data;

        // Count of single vertex components
        size_t _size;

        GLuint _channel_count;

    public:
        GLVertexBuffer(GL *gl);
        GLVertexBuffer(const GLVertexBuffer &) = delete;
        GLVertexBuffer(GLVertexBuffer &&) noexcept;
        GLVertexBuffer& operator=(const GLVertexBuffer &) = delete;
        GLVertexBuffer& operator=(GLVertexBuffer &&) noexcept;
        virtual ~GLVertexBuffer();

        RendererType get_renderer_type() const override;
        void set_data(size_t offset, const void *verts, size_t count) override;

        GLuint get_handle() const;
    };
}
