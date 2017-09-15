#pragma once
#include <giygas/Vector2.hpp>
#include <giygas/Vector4.hpp>
#include <cstddef>

namespace giygas {
    class GIYGAS_EXPORT SpriteInfo {
    public:
        Vector2 position;
        Vector2 size;
        Vector4 color;
        size_t texture_index;
    };
}

