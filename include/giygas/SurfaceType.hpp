#pragma once
#include <type_traits>
#include <giygas/export.h>

namespace giygas {
    enum class SurfaceType {
        Color       = 1,
        Depth       = 1 << 1,
        Stencil     = 1 << 2
    };

    using T = std::underlying_type<SurfaceType>::type;

    GIYGAS_EXPORT SurfaceType operator|(SurfaceType lhs, SurfaceType rhs);
    GIYGAS_EXPORT SurfaceType operator&(SurfaceType lhs, SurfaceType rhs);
    GIYGAS_EXPORT SurfaceType &operator|=(SurfaceType& lhs, SurfaceType rhs);
    GIYGAS_EXPORT bool has_flag(SurfaceType lhs, SurfaceType rhs);

}
