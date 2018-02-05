#pragma once
#include "Vector4.hpp"
#include "AttachmentType.hpp"
#include "IndexBuffer.hpp"
#include "Material.hpp"
#include "IndexRange.hpp"
#include "PipelineOptions.hpp"


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
        virtual void clear(AttachmentType surfaces) = 0;
    };
}

#undef GIYGAS_SURFACE_DRAW_FUNC
