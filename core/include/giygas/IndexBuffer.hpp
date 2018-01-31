#pragma once
#include <cstddef>
#include <giygas/export.h>
#include <cstdint>
#include "RendererType.hpp"

namespace giygas {

    template <typename T>
    class GIYGAS_EXPORT IndexBuffer {
    public:
        virtual ~IndexBuffer() = default;
        virtual RendererType get_renderer_type() const = 0;
        virtual void set(size_t offset, const T *indices, size_t count) = 0;
        virtual size_t count() const = 0;
    };

    typedef IndexBuffer<uint32_t> IndexBuffer32;
    typedef IndexBuffer<uint16_t> IndexBuffer16;
    typedef IndexBuffer<uint8_t> IndexBuffer8;
}
