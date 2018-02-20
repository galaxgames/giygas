#pragma once
#include "CommandBuffer.hpp"

namespace giygas {

    class CommandPool {
    public:
        virtual ~CommandPool() = default;
        virtual CommandBuffer *make_buffer() = 0;
        virtual void reset_buffers() = 0;
    };

}