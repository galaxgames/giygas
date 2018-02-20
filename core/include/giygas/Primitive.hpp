#pragma once
#include <giygas/export.h>

namespace giygas {
    enum class GIYGAS_EXPORT Primitive {
        Points,
        LineStrip,
        LineLoop,
        Lines,
        TriangleStrip,
        TriangleFan,
        Triangles
    };
}
