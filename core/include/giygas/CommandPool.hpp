#pragma once
#include "CommandBuffer.hpp"

namespace giygas {


    class CommandPool {
    public:
        virtual ~CommandPool() = default;
        virtual CommandBuffer take_static_buffer() = 0;
        virtual CommandBuffer take_dynamic_buffer() = 0;
    };


}