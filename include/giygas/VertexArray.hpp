#pragma once

#include "VertexBuffer.hpp"
#include "RendererType.hpp"

namespace giygas {
    class VertexArray {
    public:
        virtual ~VertexArray() = default;

        virtual RendererType get_renderer_type() const = 0;

        virtual void add_buffer(
            const VertexBuffer *buffer, const VertexBufferLayout &layout
        ) = 0;
    };
}
