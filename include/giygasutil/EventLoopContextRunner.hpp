#pragma once
#include <giygasutil/export.h>
#include <giygas/EventLoopContext.hpp>
#include "EventLoopUpdatable.hpp"

namespace giygas {
    class GIYGASUTIL_EXPORT EventLoopContextRunner {
        EventLoopContext *_context;
        EventLoopUpdatable *_updatable;
    public:
        EventLoopContextRunner(
            EventLoopContext *context,
            EventLoopUpdatable *updatable
        );
        void set_context(EventLoopContext *context);
        void set_updatable(EventLoopUpdatable *updatable);
        void run();
    };
}