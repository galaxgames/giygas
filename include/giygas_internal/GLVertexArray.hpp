#pragma once

#include <glad/glad.h>
#include <giygas_internal/GL.hpp>
#include <giygas/VertexArray.hpp>

namespace giygas {

    class GLVertexArray : public VertexArray {
        GL *_gl;
        GLuint _handle;
        GLuint _next_index;

    public:
        GLVertexArray(GL *gl);
        GLVertexArray(const GLVertexArray &);
        GLVertexArray(GLVertexArray &&) noexcept;
        GLVertexArray &operator=(const GLVertexArray &) = delete;
        GLVertexArray &operator=(GLVertexArray &&) noexcept;
        virtual ~GLVertexArray();

        RendererType get_renderer_type() const override;

        void add_buffer(
            const VertexBuffer *buffer, const VertexAttributeLayout &layout
        ) override;

        GLuint get_handle() const;
    };

}
