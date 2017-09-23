#include <giygas/SurfaceType.hpp>

namespace giygas {
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
