#pragma once
#include <giygas/export.h>
#include <giygas/Vector3.hpp>

namespace giygas  {
    class GIYGAS_EXPORT Vector4 {
    public:
        float x, y, z, w;

        Vector4();
        Vector4(float x, float y, float z, float w);

        operator Vector3() const;

    };
}
