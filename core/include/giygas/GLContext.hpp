#pragma once
#include <giygas/export.h>
#include "Context.hpp"
#include "GLVersion.hpp"

namespace giygas {
    class GIYGAS_EXPORT GLContext {
    public:
        virtual void initialize_for_opengl(
            GLVersion min_version,
            GLVersion max_version
        ) = 0;
        virtual void make_current_on_calling_thread() = 0;
        virtual void present() = 0;
    };
}