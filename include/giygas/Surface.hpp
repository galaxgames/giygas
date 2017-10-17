#pragma once
#include "Vector4.hpp"
#include "SurfaceBufferType.hpp"
#include "ElementBuffer.hpp"
#include "Material.hpp"
#include "VertexArray.hpp"
#include "ElementDrawInfo.hpp"

namespace giygas {

    class GIYGAS_EXPORT Surface {
    public:
        virtual ~Surface() = default;
        virtual void set_clear_color(Vector4 color) = 0;
        virtual void set_clear_depth(double value) = 0;
        virtual void set_clear_stencil(int value) = 0;
        virtual void clear(SurfaceBufferType surfaces) = 0;
        virtual void draw(
            VertexArray *vao, ElementBuffer<unsigned int> *ebo, Material *material,
            ElementDrawInfo element_info
        ) = 0;
        virtual void draw(
            VertexArray *vao, ElementBuffer<unsigned short> *ebo, Material *material,
            ElementDrawInfo element_info
        ) = 0;
        virtual void draw(
            VertexArray *vao, ElementBuffer<unsigned char> *ebo, Material *material,
            ElementDrawInfo element_info
        ) = 0;
    };
}