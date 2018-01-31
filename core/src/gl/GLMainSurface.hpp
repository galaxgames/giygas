#pragma once
#include <giygas/Surface.hpp>
#include "GL.hpp"
#include "GLSurfaceRenderer.hpp"

namespace giygas {

    class GLRenderer;

    class GLMainSurface : public Surface {
        GLRenderer *_renderer;
        GLSurfaceRenderer _surface_renderer;
        unsigned int _width;
        unsigned int _height;
    public:
        GLMainSurface();
        GLMainSurface(GLRenderer *gl);
        GLMainSurface(GLMainSurface &&) noexcept;
        GLMainSurface &operator=(GLMainSurface &&) noexcept;
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
        void clear(AttachmentType surfaces) override;
        GIYGAS_SURFACE_DRAW_FUNC_OVERRIDE(unsigned int);
        GIYGAS_SURFACE_DRAW_FUNC_OVERRIDE(unsigned short);
        GIYGAS_SURFACE_DRAW_FUNC_OVERRIDE(unsigned char);
        void set_size(unsigned int width, unsigned int height);
    };
}
