#pragma once
#include <giygasutil/export.h>
#include <giygas/Context.hpp>
#include "GameLoopDelegate.hpp"

namespace giygas {
    class GIYGASUTIL_EXPORT GameLoopRunner {
        Context *_context;
        GameLoopDelegate *_updatable;
    public:
        GameLoopRunner(
            Context *context,
            GameLoopDelegate *updatable
        );
        void set_context(Context *context);
        void set_updatable(GameLoopDelegate *updatable);
        void run();
    };
}