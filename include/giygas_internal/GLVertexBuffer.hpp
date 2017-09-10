#pragma once
#include <glad/glad.h>
#include <giygas/GL.hpp>
#include <giygas/VertexBuffer.hpp>

namespace giygas {
    class GLVertexBuffer : public VertexBuffer {

        GL *_gl;

        // handle to the GL buffer
        GLuint _handle;

        // The internal buffer
        float* _data;

        // Count of single vertex components
        size_t _length;

        GLuint _channel_count;

    public:
        GLVertexBuffer(GL *gl);
        GLVertexBuffer(const GLVertexBuffer &) = delete;
        GLVertexBuffer(GLVertexBuffer &&) noexcept;
        GLVertexBuffer& operator=(const GLVertexBuffer &) = delete;
        GLVertexBuffer& operator=(GLVertexBuffer &&) noexcept;
        virtual ~GLVertexBuffer();

        RendererType get_renderer_type() const override;
        void set_data(size_t offset, const float *verts, size_t count) override;
        void set_data(size_t offset, const Vector4* verts, size_t count) override;

        GLuint get_handle() const;
    };
}
