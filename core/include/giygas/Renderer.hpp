#pragma once
#include <giygas/export.h>
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "ElementBuffer.hpp"
#include "Material.hpp"
#include "SurfaceBufferType.hpp"
#include "Surface.hpp"
#include "FrameBufferSurface.hpp"
#include "RendererInitOptions.hpp"
#include "TextureInitOptions.hpp"

namespace giygas {

    class GIYGAS_EXPORT Renderer {
    public:
        virtual ~Renderer() = default;
        virtual void initialize(RendererInitOptions options) = 0;

        virtual void set_polygon_culling_enabled(bool value) = 0;
        virtual void set_polygon_culling_mode(PolygonCullingMode value) = 0;
        virtual void set_front_face_winding(VertexWinding value) = 0;

        virtual void set_depth_test_enabled(bool value) = 0;
        virtual void set_depth_mask_enabled(bool value) = 0;
        virtual void set_depth_function(DepthFunction value) = 0;
        virtual void set_depth_range(double near, double far) = 0;

        virtual VertexBuffer *make_vbo() = 0;
        virtual VertexArray *make_vao() = 0;
        virtual ElementBuffer<unsigned int> *make_int_ebo() = 0;
        virtual ElementBuffer<unsigned short> *make_short_ebo() = 0;
        virtual ElementBuffer<unsigned char> *make_char_ebo() = 0;
        virtual Material *make_material() = 0;
        virtual Shader *make_shader() = 0;
        virtual Texture *make_texture(TextureInitOptions options) = 0;
        virtual FrameBufferSurface *make_framebuffer() = 0;
        virtual RenderBuffer *make_renderbuffer() = 0;
        virtual Surface *main_surface() = 0;

        virtual void present() = 0;
    };
}


