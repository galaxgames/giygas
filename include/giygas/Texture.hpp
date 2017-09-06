#pragma once
#include <giygas/export.h>
#include <giygas/TextureFormat.hpp>

namespace giygas {
    class GIYGAS_EXPORT Texture {
    public:
        virtual ~Texture() = default;

        virtual void set_data(
            const char *data, int size, int width, int height,
            TextureFormat format
        ) = 0;
    };
}

