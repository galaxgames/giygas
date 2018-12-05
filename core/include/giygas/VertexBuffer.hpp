#pragma once
#include <giygas/Vector2.hpp>
#include <giygas/Vector3.hpp>
#include <giygas/Vector4.hpp>
#include "VertexAttributeLayout.hpp"
#include "RendererType.hpp"
#include <cstdint>
#include <cassert>

namespace giygas {

    enum VertexBufferCreateFlags {
        VertexBufferCreateFlag_None     = 0,
        VertexBufferCreateFlag_Writable = 1 << 0
    };

    class GIYGAS_EXPORT VertexBuffer {
    public:
        virtual ~VertexBuffer() = default;
        virtual RendererType renderer_type() const = 0;
        virtual void *cast_to_renderer_specific() = 0;

        /**
         * Copy data to the VBO after creation. This may only be called if VertexBufferCreateFlags::Writable was
         * included in the creation flags.
         *
         * @brief set_data  copy data to the vbo
         * @param offset    byte offset of the vbo to copy data to.
         * @param data      pointer to data to copy.
         * @param size      count bytes to copy
         */
        virtual void set_data(uint32_t offset, const uint8_t *data, uint32_t size) = 0;

        virtual bool is_valid() const = 0;
        virtual bool is_writable() const = 0;

        template <typename T>
        static void set_data(VertexBuffer &buffer, uint32_t offset, const T *data, uint32_t count) {
            assert(sizeof(T) % sizeof(uint8_t) == 0);
            size_t size = sizeof(T) / sizeof(uint8_t) * count;
            buffer.set_data(offset, reinterpret_cast<const uint8_t *>(data), size);
        }
    };

}



