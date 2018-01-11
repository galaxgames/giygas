#pragma once
#include <glad/glad.h>
#include "GL.hpp"
#include <giygas/VertexArray.hpp>

namespace giygas {

    class GLRenderer;

    class GLVertexArray : public VertexArray {
        GLRenderer *_renderer;
        GLuint _handle;
        GLuint _next_index;

    public:
        GLVertexArray(GLRenderer *renderer);
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
