#pragma once
#include <giygas/Vector2.hpp>
#include <giygas/Vector3.hpp>
#include <giygas/Vector4.hpp>
#include "VertexBufferLayout.hpp"
#include "RendererType.hpp"

namespace giygas {
    class GIYGAS_EXPORT VertexBuffer {
    public:
        virtual ~VertexBuffer() = default;
        virtual RendererType get_renderer_type() const = 0;
        virtual void set_data(size_t offset, const float* data, size_t count) = 0;
        virtual void set_data(size_t offset, const Vector4* data, size_t count) = 0;
    };
}



