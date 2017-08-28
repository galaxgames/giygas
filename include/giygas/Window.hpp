#pragma once
#include <giygas/export.h>

namespace giygas {
    class GIYGAS_EXPORT Window {
    public:
        virtual void show() = 0;
        virtual void update() = 0;
        virtual bool should_close() const = 0;
        virtual bool is_valid() const = 0;
    };
}