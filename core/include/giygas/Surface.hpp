#pragma once
#include "Vector4.hpp"
#include "SurfaceBufferType.hpp"
#include "ElementBuffer.hpp"
#include "Material.hpp"
#include "VertexArray.hpp"
#include "ElementDrawInfo.hpp"
#include "PipelineOptions.hpp"


#define GIYGAS_SURFACE_DRAW_FUNC(ebo_type) \
        virtual void draw( \
            VertexArray *vao, \
            ElementBuffer<ebo_type> *ebo, \
            Material *material, \
            ElementDrawInfo element_info \
        ) = 0

#define GIYGAS_SURFACE_DRAW_FUNC_OVERRIDE(ebo_type) \
        void draw( \
            VertexArray *vao, \
            ElementBuffer<ebo_type> *ebo, \
            Material *material, \
            ElementDrawInfo element_info \
        ) override

namespace giygas {

    class GIYGAS_EXPORT Surface {
    public:
        virtual ~Surface() = default;
        virtual unsigned int width() const = 0;
        virtual unsigned int height() const = 0;
        virtual void set_viewport(
            unsigned int x,
            unsigned int y,
            unsigned int width,
            unsigned int height
        ) = 0;
        virtual void set_clear_color(Vector4 color) = 0;
        virtual void set_clear_depth(double value) = 0;
        virtual void set_clear_stencil(int value) = 0;
        virtual void clear(SurfaceBufferType surfaces) = 0;
        GIYGAS_SURFACE_DRAW_FUNC(unsigned int);
        GIYGAS_SURFACE_DRAW_FUNC(unsigned short);
        GIYGAS_SURFACE_DRAW_FUNC(unsigned char);
    };
}

#undef GIYGAS_SURFACE_DRAW_FUNC
