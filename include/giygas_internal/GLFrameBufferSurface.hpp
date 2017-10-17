#pragma once
#include <giygas/FrameBufferSurface.hpp>
#include "GL.hpp"
#include "GLSurfaceRenderer.hpp"

namespace giygas {
    class GLFrameBufferSurface : public FrameBufferSurface {
        GL *_gl;
        GLuint _handle;
        GLSurfaceRenderer _renderer;

        GLenum get_attachment_flag(SurfaceBufferType attachment_type) const;

    public:
        GLFrameBufferSurface(GL *gl);
        GLFrameBufferSurface(const GLFrameBufferSurface &) = delete;
        GLFrameBufferSurface(GLFrameBufferSurface &&) noexcept;
        GLFrameBufferSurface &operator=(const GLFrameBufferSurface &) = delete;
        GLFrameBufferSurface &operator=(GLFrameBufferSurface &&) noexcept;
        ~GLFrameBufferSurface() override;

        void attach_texture(
            Texture *texture,
            SurfaceBufferType attachment_type
        ) override;
        void attach_renderbuffer(
            RenderBuffer *renderbuffer,
            SurfaceBufferType attachment_type
        ) override;

        void set_clear_color(Vector4 color) override;
        void set_clear_depth(double value) override;
        void set_clear_stencil(int value) override;
        void clear(SurfaceBufferType surfaces) override;

        void draw(
            VertexArray *vao,
            ElementBuffer<unsigned int> *ebo,
            Material *material,
            ElementDrawInfo element_info
        ) override;

        void draw(
            VertexArray *vao,
            ElementBuffer<unsigned short> *ebo,
            Material *material,
            ElementDrawInfo element_info
        ) override;

        void draw(
            VertexArray *vao,
            ElementBuffer<unsigned char> *ebo,
            Material *material,
            ElementDrawInfo element_info
        ) override;
    };
}
