#pragma once
#include "GL.hpp"
#include <giygas/RenderBuffer.hpp>

namespace giygas {
    class GLRenderBuffer : public RenderBuffer {
        GL *_gl;
        GLuint _handle;
    public:
        GLRenderBuffer(GL *gl);
        GLRenderBuffer(GLRenderBuffer &&) noexcept;
        GLRenderBuffer &operator=(GLRenderBuffer &&) noexcept;
        ~GLRenderBuffer() override;
        RendererType renderer_type() const override;
        void create_storage(
            unsigned int width,
            unsigned int height,
            TextureFormat format
        ) override;

        GLuint handle() const;
    };
}
