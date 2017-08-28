#pragma once
#include <giygas/export.h>

namespace giygas  {
    class GIYGAS_EXPORT Vector4 {
    public:
        Vector4(float x, float y, float z, float w);

        float x;
        float y;
        float z;
        float w;
    };
}