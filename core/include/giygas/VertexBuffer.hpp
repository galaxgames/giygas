#pragma once
#include <giygas/Vector2.hpp>
#include <giygas/Vector3.hpp>
#include <giygas/Vector4.hpp>
#include "VertexAttributeLayout.hpp"
#include "RendererType.hpp"
#include <cstdint>
#include <cassert>

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

        template <typename T>
        static void set_data(VertexBuffer &buffer, size_t offset, const T *data, size_t count) {
            assert(sizeof(T) % sizeof(uint8_t) == 0);
            size_t size = sizeof(T) / sizeof(uint8_t) * count;
            buffer.set_data(offset, reinterpret_cast<const uint8_t *>(data), size);
        }
    };
}



