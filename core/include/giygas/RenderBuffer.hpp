#pragma once

#include "TextureFormat.hpp"
#include "RendererType.hpp"

namespace giygas {
    class RenderBuffer {
    public:
        virtual ~RenderBuffer() = default;
        virtual RendererType renderer_type() const = 0;
        virtual void create_storage(
            unsigned int width,
            unsigned int height,
            TextureFormat format
        ) = 0;
    };
}
