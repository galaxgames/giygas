#pragma once
#include <giygas/export.h>
#include "Context.hpp"

namespace giygas {
    class GIYGAS_EXPORT GLContext {
    public:
        virtual void make_current_on_calling_thread() = 0;
        virtual void present() = 0;
    };
}