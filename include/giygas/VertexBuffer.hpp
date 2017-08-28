#pragma once
#include <giygas/Vector2.hpp>
#include <giygas/Vector3.hpp>
#include <giygas/Vector4.hpp>
#include "VertexBufferLayout.hpp"

namespace giygas {
    class GIYGAS_EXPORT VertexBuffer {
    public:
        virtual void set_layout(const VertexBufferLayout& layout) = 0;
        virtual bool has_layout() const = 0;
        virtual void set_vector2_channel(int index, const Vector2* verts, int count) = 0;
        virtual void set_vector3_channel(int index, const Vector3* verts, int count) = 0;
        virtual void set_vector4_channel(int index, const Vector4* verts, int count) = 0;
    };
}



