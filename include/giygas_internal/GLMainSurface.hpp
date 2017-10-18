#pragma once
#include <giygas/Surface.hpp>
#include "GL.hpp"
#include "GLSurfaceRenderer.hpp"

namespace giygas {
    class GLMainSurface : public Surface {
        GL *_gl;
        GLSurfaceRenderer _renderer;
        unsigned int _width;
        unsigned int _height;
    public:
        GLMainSurface(GL *gl);
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

        void set_size(unsigned int width, unsigned int height);
    };
}
