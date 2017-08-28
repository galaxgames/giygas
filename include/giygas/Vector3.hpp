#pragma once
#include <giygas/export.h>

namespace giygas {
    class GIYGAS_EXPORT Vector3 {
    public:
        Vector3(float x, float y, float z);

        float x;
        float y;
        float z;
    };

}