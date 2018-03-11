#pragma once
#include <giygasutil/export.h>
#include <giygas/EventLoopContext.hpp>
#include "GameLoopDelegate.hpp"

namespace giygas {
    class GIYGASUTIL_EXPORT EventLoopContextRunner {
        EventLoopContext *_context;
        GameLoopDelegate *_updatable;
    public:
        EventLoopContextRunner(
            EventLoopContext *context,
            GameLoopDelegate *updatable
        );
        void set_context(EventLoopContext *context);
        void set_updatable(GameLoopDelegate *updatable);
        void run();
    };
}