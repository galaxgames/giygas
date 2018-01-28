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

        GLFWWindowInitOptions();

        GLFWWindowInitOptions(
            bool is_resizable,
            const char *title,
            unsigned int width,
            unsigned int height
        );

    };
}
