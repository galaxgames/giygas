#pragma once
#include <giygas/Renderer.hpp>
#include <giygas/Primitive.hpp>
#include "Window.hpp"
#include "GLImpl.hpp"


namespace giygas {

    class GenericGLElementBuffer;

    class GIYGAS_EXPORT GLRenderer : public Renderer {
        GLImpl _gl;
        std::shared_ptr<Window> _window;

        static GLenum get_gl_primitive(Primitive primitive);
        void draw_internal(
            VertexArray *vao, GenericGLElementBuffer *ebo, Material *material,
            ElementDrawInfo element_info, GLenum element_type
        );

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

        void clear() override;
        void draw(
            VertexArray *vao, ElementBuffer<unsigned int> *ebo, Material *material,
            ElementDrawInfo element_info
        ) override;
        void draw(
            VertexArray *vao, ElementBuffer<unsigned short> *ebo, Material *material,
            ElementDrawInfo element_info
        ) override;
        void draw(
            VertexArray *vao, ElementBuffer<unsigned char> *ebo, Material *material,
            ElementDrawInfo element_info
        ) override;

    };

}
