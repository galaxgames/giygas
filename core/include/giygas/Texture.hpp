#pragma once
#include <cstddef>
#include <cstdint>
#include <giygas/export.h>
#include <giygas/TextureFormat.hpp>
#include "RendererType.hpp"
#include "SamplerOptions.hpp"

namespace giygas {
    class GIYGAS_EXPORT Texture {
    public:
        virtual ~Texture() = default;

        virtual RendererType renderer_type() const = 0;

        virtual void set_data(
            const uint8_t *data,
            size_t size,
            uint32_t width,
            uint32_t height,
            TextureFormat format
        ) = 0;

        virtual void create_storage(
            uint32_t width,
            uint32_t height,
            TextureFormat format
        ) = 0;

    };
}

