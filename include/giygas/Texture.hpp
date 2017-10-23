#pragma once
#include <cstddef>
#include <giygas/export.h>
#include <giygas/TextureFormat.hpp>
#include "RendererType.hpp"
#include "TextureInitOptions.hpp"

namespace giygas {
    class GIYGAS_EXPORT Texture {
    public:
        virtual ~Texture() = default;

        virtual RendererType renderer_type() const = 0;

        virtual void set_data(
            const unsigned char *data,
            size_t size,
            size_t width,
            size_t height,
            TextureFormat format
        ) = 0;

        virtual void create_storage(
            size_t width,
            size_t height,
            TextureFormat format
        ) = 0;

        virtual void set_wrap_mode_u(TextureWrapMode mode) = 0;
        virtual void set_wrap_mode_v(TextureWrapMode mode) = 0;
        virtual void set_minify_filter_mode(TextureMinifyFilterMode mode) = 0;
        virtual void set_magnify_filter_mode(TextureMagnifyFilterMode mode) = 0;
    };
}

