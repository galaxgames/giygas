#pragma once
#include <type_traits>
#include <giygas/export.h>

namespace giygas {
    enum class SurfaceBufferType {
        Color       = 1,
        Depth       = 1 << 1,
        Stencil     = 1 << 2
    };

    using T = std::underlying_type<SurfaceBufferType>::type;

    GIYGAS_EXPORT SurfaceBufferType operator|(SurfaceBufferType lhs, SurfaceBufferType rhs);
    GIYGAS_EXPORT SurfaceBufferType operator&(SurfaceBufferType lhs, SurfaceBufferType rhs);
    GIYGAS_EXPORT SurfaceBufferType &operator|=(SurfaceBufferType& lhs, SurfaceBufferType rhs);
    GIYGAS_EXPORT bool has_flag(SurfaceBufferType lhs, SurfaceBufferType rhs);

}
