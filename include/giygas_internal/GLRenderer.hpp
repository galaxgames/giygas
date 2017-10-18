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
        virtual ~GLRenderer();

        VertexBuffer *make_vbo() override;
        ElementBuffer<unsigned int> *make_int_ebo() override;
        ElementBuffer<unsigned short> *make_short_ebo() override;
        ElementBuffer<unsigned char> *make_char_ebo() override;
        VertexArray *make_vao() override;
        Material *make_material() override;
        Shader *make_shader() override;
        Texture *make_texture() override;
        FrameBufferSurface *make_framebuffer() override;
        RenderBuffer *make_renderbuffer() override;
        Surface *main_surface() override;
    };

}
