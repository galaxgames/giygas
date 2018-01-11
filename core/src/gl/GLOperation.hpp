#pragma once

#include "GL.hpp"

namespace giygas {
    class GLOperation {
    public:
        virtual ~GLOperation() = default;
        virtual void execute(GL* gl) = 0;
    };
}
