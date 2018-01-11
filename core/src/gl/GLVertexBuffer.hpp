#pragma once
#include <glad/glad.h>
#include "GL.hpp"
#include "GLRenderer.hpp"
#include "operations/GenBufferGLOperation.hpp"
#include <giygas/VertexBuffer.hpp>
#include <stack>

namespace giygas {
    using namespace std;

    class GLVertexBuffer : public VertexBuffer {
        typedef unsigned char byte;

        GLRenderer *_renderer;

        //handle to the GL buffer
        GLuint _handle;

        // The internal buffer
        byte *_data;

        // Count of single vertex components
        size_t _size;

        GLuint _channel_count;


    public:
        GLVertexBuffer(GLRenderer *renderer);
        GLVertexBuffer(const GLVertexBuffer &) = delete;
        GLVertexBuffer(GLVertexBuffer &&) noexcept;
        GLVertexBuffer& operator=(const GLVertexBuffer &) = delete;
        GLVertexBuffer& operator=(GLVertexBuffer &&) noexcept;
        virtual ~GLVertexBuffer();

        RendererType get_renderer_type() const override;
        void set_data(size_t offset, const void *verts, size_t count) override;

        GLuint get_handle() const;

        void set_handle(GLuint handle);
    };
}
