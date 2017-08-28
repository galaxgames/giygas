#pragma once
#include <giygas/export.h>
#include "VertexBuffer.hpp"
#include "ElementBuffer.hpp"
#include "Material.hpp"

namespace giygas {
    class GIYGAS_EXPORT Renderer {
    public:
        virtual VertexBuffer* make_vbo() = 0;
        virtual ElementBuffer* make_ebo() = 0;
        virtual Material* make_material() = 0;
        virtual Shader* make_shader() = 0;

        virtual void clear() = 0;
        virtual void draw(
            VertexBuffer *vbo, ElementBuffer *ebo, Material *material
        ) = 0;

    };
}


