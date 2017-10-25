#pragma once
#include <giygas/export.h>
#include "Context.hpp"

namespace giygas {
    class GIYGAS_EXPORT EventLoopContext : public Context {
    public:
        virtual ~EventLoopContext() = default;
        virtual void update() = 0;
        virtual bool should_close() const = 0;
    };
}