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
    class GLSurfaceRenderer {
        Vector4 _clear_color;
        double _clear_depth;
        int _clear_stencil;

        static GLenum get_gl_primitive(Primitive primitive);
        static GLenum get_clear_flags(SurfaceBufferType surfaces);
        void draw_internal(
            GL *gl,
            VertexArray *vao,
            GenericGLElementBuffer *ebo,
            Material *material,
            ElementDrawInfo element_info,
            GLenum element_type
        );

    public:
        void set_clear_color(Vector4 color);
        void set_clear_depth(double value);
        void set_clear_stencil(int value);
        void clear(SurfaceBufferType surfaces);
        void draw(
            GL *gl,
            VertexArray *vao,
            ElementBuffer<unsigned int> *ebo,
            Material *material,
            ElementDrawInfo element_info
        );
        void draw(
            GL *gl,
            VertexArray *vao,
            ElementBuffer<unsigned short> *ebo,
            Material *material,
            ElementDrawInfo element_info
        );
        void draw(
            GL *gl,
            VertexArray *vao,
            ElementBuffer<unsigned char> *ebo,
            Material *material,
            ElementDrawInfo element_info
        );
    };
}