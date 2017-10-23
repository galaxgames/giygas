#pragma once
#include <giygas/Renderer.hpp>
#include <giygas/Primitive.hpp>
#include "giygas/Window.hpp"
#include "GLImpl.hpp"
#include "giygas/Primitive.hpp"
#include "GLMainSurface.hpp"

namespace giygas {

    class GenericGLElementBuffer;

    class GIYGAS_EXPORT GLRenderer : public Renderer, public SurfaceSizeChangedListener {
        GLImpl _gl;
        GLMainSurface _main_surface;
        std::shared_ptr<Window> _window;

#ifndef NDEBUG
        bool _initialized;
#endif

        void handle_surface_size_changed(
            unsigned int width,
            unsigned int height
        ) override;

        void move_common(GLRenderer &&other) noexcept;

    public:
        GLRenderer(std::shared_ptr<Window> window);
        GLRenderer(const GLRenderer &) = delete;
        GLRenderer(GLRenderer &&) noexcept;
        GLRenderer& operator=(const GLRenderer &) = delete;
        GLRenderer& operator=(GLRenderer &&) noexcept;
        ~GLRenderer();

        void initialize(RendererInitOptions options) override;
        void set_polygon_culling_enabled(bool value) override;
        void set_polygon_culling_mode(PolygonCullingMode value) override;
        void set_front_face_winding(VertexWinding value) override;
        VertexBuffer *make_vbo() override;
        ElementBuffer<unsigned int> *make_int_ebo() override;
        ElementBuffer<unsigned short> *make_short_ebo() override;
        ElementBuffer<unsigned char> *make_char_ebo() override;
        VertexArray *make_vao() override;
        Material *make_material() override;
        Shader *make_shader() override;
        Texture *make_texture(TextureInitOptions options) override;
        FrameBufferSurface *make_framebuffer() override;
        RenderBuffer *make_renderbuffer() override;
        Surface *main_surface() override;
    };

}
