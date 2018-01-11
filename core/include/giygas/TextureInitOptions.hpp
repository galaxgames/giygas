#pragma once
#include <giygas/export.h>

namespace giygas {

    enum class GIYGAS_EXPORT TextureWrapMode {
        ClampToEdge,
        ClampToBorder,
        MirroredRepeat,
        Repeat
    };

    enum class GIYGAS_EXPORT TextureMinifyFilterMode {
        Nearest,
        Linear,
        NearestMipMapNearest,
        LinearMipMapNearest,
        NearestMipMapLinear,
        LinearMipMapLinear
    };

    enum class GIYGAS_EXPORT TextureMagnifyFilterMode {
        Nearest,
        Linear
    };

    class GIYGAS_EXPORT TextureInitOptions {
    public:
        TextureWrapMode wrap_mode_u;
        TextureWrapMode wrap_mode_v;
        TextureMinifyFilterMode  minify_filter_mode;
        TextureMagnifyFilterMode magnify_filter_mode;

        TextureInitOptions();

        TextureInitOptions(
            TextureWrapMode wrap_mode,
            TextureMinifyFilterMode minify_filter_mode,
            TextureMagnifyFilterMode magnify_filter_mode
        );

        TextureInitOptions(
            TextureWrapMode wrap_mode_u,
            TextureWrapMode wrap_mode_v,
            TextureMinifyFilterMode minify_filter_mode,
            TextureMagnifyFilterMode magnify_filter_mode
        );
    };
}