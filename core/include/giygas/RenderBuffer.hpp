#pragma once
#include <cstdint>
#include "TextureFormat.hpp"
#include "RendererType.hpp"

namespace giygas {
    class RenderBuffer {
    public:
        virtual ~RenderBuffer() = default;
        virtual RendererType renderer_type() const = 0;
        virtual void create_storage(
            uint32_t width,
            uint32_t height,
            TextureFormat format
        ) = 0;
    };
}
