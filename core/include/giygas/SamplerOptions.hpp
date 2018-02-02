#pragma once
#include <giygas/export.h>

namespace giygas {

    enum class GIYGAS_EXPORT SamplerWrapMode {
        ClampToEdge,
        ClampToBorder,
        MirroredRepeat,
        Repeat
    };

    enum class GIYGAS_EXPORT SamplerFilterMode {
        Nearest,
        Linear
    };

    enum class GIYGAS_EXPORT SamplerMipmapMode {
        Nearest,
        Linear
    };

    class GIYGAS_EXPORT SamplerOptions {
    public:
        SamplerWrapMode wrap_mode_u;
        SamplerWrapMode wrap_mode_v;
        SamplerFilterMode  minify_filter_mode;
        SamplerFilterMode magnify_filter_mode;
        SamplerMipmapMode mipmap_mode;
    };
}