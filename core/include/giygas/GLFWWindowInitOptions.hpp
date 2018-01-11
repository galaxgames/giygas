#pragma once
#include <giygas/export.h>
#include "GLVersion.hpp"

namespace giygas {

    class GIYGAS_EXPORT GLFWWindowInitOptions {
    public:
        bool is_resizable;
        const char *title;
        unsigned int width;
        unsigned int height;
        GLVersion gl_min_version;
        GLVersion gl_max_version;

        GLFWWindowInitOptions();

        GLFWWindowInitOptions(
            bool is_resizable,
            const char *title,
            unsigned int width,
            unsigned int height
        );

        GLFWWindowInitOptions(
            bool is_resizable,
            const char *title,
            unsigned int width,
            unsigned int height,
            GLVersion gl_min_version
        );

        GLFWWindowInitOptions(
            bool is_resizable,
            const char *title,
            unsigned int width,
            unsigned int height,
            GLVersion gl_min_version,
            GLVersion gl_max_version
        );
    };
}
