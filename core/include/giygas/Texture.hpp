#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>
#include <giygas/export.h>
#include <giygas/TextureFormat.hpp>
#include "RendererType.hpp"
#include "SamplerParameters.hpp"
#include "RenderTarget.hpp"

namespace giygas {
    using namespace std;

    // TODO: Make this an enum class
    enum TextureUsageFlags {
        TEXTURE_USAGE_NONE = 0,
        TEXTURE_USAGE_SAMPLE = 1<<0,
        TEXTURE_USAGE_COLOR_ATTACHMENT = 1<<1,
        TEXTURE_USAGE_DEPTH_ATTACHMENT = 1<<2,
        TEXTURE_USAGE_STENCIL_ATTACHMENT = 1<<3
    };

    class GIYGAS_EXPORT Texture : public RenderTarget {
    public:
        virtual ~Texture() = default;

        virtual void create(
            unique_ptr<uint8_t[]> &&data,
            size_t size,
            uint32_t width,
            uint32_t height,
            TextureFormat input_format,
            TextureFormat desired_format,
            TextureUsageFlags flags
        ) = 0;

        virtual TextureFormat format() const = 0;
        virtual const void *texture_impl() const = 0;

    };
}

