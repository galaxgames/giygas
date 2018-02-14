#pragma once

#include <giygas/CommandPool.hpp>

namespace giygas {

    class CommandBufferImpl {
    public:
        virtual ~CommandBufferImpl() = default;
        virtual RendererType renderer_type() const = 0;
        virtual void release() = 0;
        virtual void record(const DrawInfo &info) = 0;
    };

}