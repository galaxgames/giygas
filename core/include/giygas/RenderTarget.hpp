#pragma once
#include "RendererType.hpp"
#include <cstdint>

namespace giygas {

    class RenderTarget {
    public:
        virtual ~RenderTarget() = default;
        virtual RendererType renderer_type() const = 0;
        virtual const void *rendertarget_impl() const = 0;

        virtual uint32_t width() const = 0;
        virtual uint32_t height() const = 0;
    };

}
