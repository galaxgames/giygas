#pragma once
#include <giygasutil/export.h>

namespace giygas {
    class GIYGASUTIL_EXPORT EventLoopUpdatable {
    public:
        virtual ~EventLoopUpdatable() = default;
        virtual void update(float elapsed_seconds) = 0;
    };
}