#pragma once
#include "RendererType.hpp"

namespace giygas {

    class CommandPool {
    public:
        virtual ~CommandPool() = default;
        virtual RendererType renderer_type() const = 0;
        virtual void create() = 0;
        virtual void reset_buffers() = 0;
        virtual bool is_valid() const = 0;
    };

}