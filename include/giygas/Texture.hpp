#pragma once
#include <giygas/export.h>
#include <giygas/TextureFormat.hpp>
#include "RendererType.hpp"

namespace giygas {
    class GIYGAS_EXPORT Texture {
    public:
        virtual ~Texture() = default;

        virtual RendererType renderer_type() const = 0;

        virtual void set_data(
            const unsigned char *data, size_t size, size_t width, size_t height,
            TextureFormat format
        ) = 0;
    };
}

