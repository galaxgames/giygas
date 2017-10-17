#pragma once
#include <giygas/export.h>
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "ElementBuffer.hpp"
#include "Material.hpp"
#include "SurfaceBufferType.hpp"
#include "Surface.hpp"
#include "FrameBufferSurface.hpp"

namespace giygas {

    class GIYGAS_EXPORT Renderer {
    public:
        virtual ~Renderer() = default;
        virtual VertexBuffer *make_vbo() = 0;
        virtual VertexArray *make_vao() = 0;
        virtual ElementBuffer<unsigned int> *make_int_ebo() = 0;
        virtual ElementBuffer<unsigned short> *make_short_ebo() = 0;
        virtual ElementBuffer<unsigned char> *make_char_ebo() = 0;
        virtual Material *make_material() = 0;
        virtual Shader *make_shader() = 0;
        virtual Texture *make_texture() = 0;
        virtual FrameBufferSurface *make_framebuffer() = 0;
        virtual RenderBuffer *make_renderbuffer() = 0;
        virtual Surface *main_surface() = 0;
    };
}


