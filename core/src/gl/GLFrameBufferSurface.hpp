#pragma once
#include <giygas/FrameBufferSurface.hpp>
#include "GL.hpp"
#include "GLSurfaceRenderer.hpp"

namespace giygas {
    using namespace std;

    class GLRenderer;

    class GLFrameBufferSurface : public FrameBufferSurface {
        GLRenderer *_renderer;
        GLuint _handle;
        GLSurfaceRenderer _surface_renderer;
        unsigned int _width;
        unsigned int _height;

        void move_common(GLFrameBufferSurface &&other) noexcept;
        GLenum get_attachment_flag(SurfaceBufferType attachment_type) const;

    public:
        GLFrameBufferSurface(GLRenderer *renderer);
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

        void set_size(unsigned int width, unsigned int height);

        unsigned int width() const override;
        unsigned int height() const override;

        void set_viewport(
            unsigned int x,
            unsigned int y,
            unsigned int width,
            unsigned int height
        ) override;

        void set_clear_color(Vector4 color) override;
        void set_clear_depth(double value) override;
        void set_clear_stencil(int value) override;
        void clear(SurfaceBufferType surfaces) override;
        GIYGAS_SURFACE_DRAW_FUNC_OVERRIDE(unsigned int);
        GIYGAS_SURFACE_DRAW_FUNC_OVERRIDE(unsigned short);
        GIYGAS_SURFACE_DRAW_FUNC_OVERRIDE(unsigned char);
    };
}
