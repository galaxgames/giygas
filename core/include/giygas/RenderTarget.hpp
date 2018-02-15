#pragma once

#include "RendererType.hpp"

namespace giygas {

    class RenderTarget {
    public:
        virtual ~RenderTarget() = default;
        virtual RendererType renderer_type() const = 0;
        virtual const void *impl() const = 0;
    };

}