#pragma once
#include <giygas/export.h>
#include <giygas/Vector3.hpp>

namespace giygas  {
    class GIYGAS_EXPORT Vector4 {
    public:
        float x, y, z, w;

        Vector4() = default; // Default in header to allow default value initialization
        Vector4(float x, float y, float z, float w);
    };
}
