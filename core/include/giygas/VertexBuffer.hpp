#pragma once
#include <giygas/Vector2.hpp>
#include <giygas/Vector3.hpp>
#include <giygas/Vector4.hpp>
#include "VertexAttributeLayout.hpp"
#include "RendererType.hpp"
#include <cstdint>

namespace giygas {
    class GIYGAS_EXPORT VertexBuffer {
    public:
        virtual ~VertexBuffer() = default;
        virtual RendererType renderer_type() const = 0;

        /**
         * @brief set_data  copy data to the vbo
         * @param offset    byte offset of the vbo to copy data to.
         * @param data      pointer to data to copy.
         * @param size      count bytes to copy
         */
        virtual void set_data(size_t offset, const uint8_t *data, size_t size) = 0;
    };
}



