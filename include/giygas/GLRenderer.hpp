#pragma once
#include <giygas/Renderer.hpp>
#include <giygas/Primitive.hpp>
#include "Window.hpp"

namespace giygas {
    class GIYGAS_EXPORT GLRenderer : public Renderer {
        std::shared_ptr<Window> _window;

        static GLenum get_gl_primitive(Primitive primitive);

    public:
        GLRenderer(std::shared_ptr<Window> window);
        GLRenderer(const GLRenderer &) = delete;
        GLRenderer(GLRenderer &&) noexcept;
        GLRenderer& operator=(const GLRenderer &) = delete;
        GLRenderer& operator=(GLRenderer &&) noexcept;
        virtual ~GLRenderer();

        VertexBuffer* make_vbo() override;
        ElementBuffer* make_ebo() override;
        Material* make_material() override;
        Shader* make_shader() override;

        void clear() override;
        void draw(
            VertexBuffer *vbo, ElementBuffer *ebo, Material *material
        ) override;

    };

}