#include <giygas/SurfaceBufferType.hpp>

namespace giygas {
    SurfaceBufferType operator|(SurfaceBufferType lhs, SurfaceBufferType rhs) {
        return static_cast<SurfaceBufferType>(static_cast<T>(lhs) | static_cast<T>(rhs));
    }

    SurfaceBufferType operator&(SurfaceBufferType lhs, SurfaceBufferType rhs) {
        return static_cast<SurfaceBufferType>(static_cast<T>(lhs) & static_cast<T>(rhs));
    }

    SurfaceBufferType &operator|=(SurfaceBufferType& lhs, SurfaceBufferType rhs) {
        lhs = static_cast<SurfaceBufferType>(static_cast<T>(lhs) | static_cast<T>(rhs));
        return lhs;
    }

    bool has_flag(SurfaceBufferType lhs, SurfaceBufferType rhs) {
        return static_cast<T>(lhs & rhs) > 0;
    }
}
