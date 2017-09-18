#pragma once
#include <type_traits>

namespace giygas {
    enum class SurfaceType {
        Color       = 1,
        Depth       = 1 << 1,
        Stencil     = 1 << 2
    };

    using T = std::underlying_type<SurfaceType>::type;

    SurfaceType operator|(SurfaceType lhs, SurfaceType rhs) {
        return static_cast<SurfaceType>(static_cast<T>(lhs) | static_cast<T>(rhs));
    }

    SurfaceType operator&(SurfaceType lhs, SurfaceType rhs) {
        return static_cast<SurfaceType>(static_cast<T>(lhs) & static_cast<T>(rhs));
    }

    SurfaceType &operator|=(SurfaceType& lhs, SurfaceType rhs) {
        lhs = static_cast<SurfaceType>(static_cast<T>(lhs) | static_cast<T>(rhs));
        return lhs;
    }

    bool has_flag(SurfaceType lhs, SurfaceType rhs) {
        return static_cast<T>(lhs & rhs) > 0;
    }

}
