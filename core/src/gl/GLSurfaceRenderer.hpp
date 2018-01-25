#pragma once
#include <giygas/Vector4.hpp>
#include <glad/glad.h>
#include <giygas/Primitive.hpp>
#include <giygas/SurfaceBufferType.hpp>
#include <giygas/VertexArray.hpp>
#include <giygas/Material.hpp>
#include <giygas/Surface.hpp>
#include "GLElementBuffer.hpp"

namespace giygas {

    class GLRenderer;

    class GLSurfaceRenderer {
        Vector4 _clear_color;
        double _clear_depth;
        int _clear_stencil;
        unsigned int _viewport_x;
        unsigned int _viewport_y;
        unsigned int _viewport_width;
        unsigned int _viewport_height;

        static GLenum get_gl_primitive(Primitive primitive);
        static GLenum get_clear_flags(SurfaceBufferType surfaces);
        void draw_internal(
            GLRenderer *renderer,
            GLuint framebuffer,
            VertexArray *vao,
            GenericGLElementBuffer *ebo,
            Material *material,
            ElementDrawInfo element_info,
            GLenum element_type
        );

    public:
        GLSurfaceRenderer();
        void set_viewport(
            unsigned int x,
            unsigned int y,
            unsigned int width,
            unsigned int height
        );
        void set_clear_color(Vector4 color);
        void set_clear_depth(double value);
        void set_clear_stencil(int value);
        void clear(
            GLRenderer *renderer,
            GLuint framebuffer,
            SurfaceBufferType surfaces
        );
        void draw(
            GLRenderer *renderer,
            GLuint framebuffer,
            VertexArray *vao,
            ElementBuffer<unsigned int> *ebo,
            Material *material,
            ElementDrawInfo element_info
        );
        void draw(
            GLRenderer *renderer,
            GLuint framebuffer,
            VertexArray *vao,
            ElementBuffer<unsigned short> *ebo,
            Material *material,
            ElementDrawInfo element_info
        );
        void draw(
            GLRenderer *renderer,
            GLuint framebuffer,
            VertexArray *vao,
            ElementBuffer<unsigned char> *ebo,
            Material *material,
            ElementDrawInfo element_info
        );
    };
}
