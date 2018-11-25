#pragma once
#include <giygasutil/export.h>

namespace giygas {
    class GIYGASUTIL_EXPORT GameLoopDelegate {
    public:
        virtual ~GameLoopDelegate() = default;
        virtual void update_logic(float elapsed_seconds) = 0;
        virtual void update_graphics() = 0;
        virtual bool should_close() const = 0;
    };
}
