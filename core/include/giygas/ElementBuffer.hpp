#pragma once
#include <cstddef>
#include <giygas/export.h>
#include <cstdint>
#include "RendererType.hpp"

namespace giygas {

    template <typename T>
    class GIYGAS_EXPORT ElementBuffer {
    public:
        virtual ~ElementBuffer() = default;
        virtual RendererType get_renderer_type() const = 0;
        virtual void set(size_t index, const T *elements, size_t count) = 0;
        virtual size_t count() const = 0;
    };

    typedef ElementBuffer<uint32_t> ElementBuffer32;
    typedef ElementBuffer<uint16_t> ElementBuffer16;
    typedef ElementBuffer<uint8_t> ElementBuffer8;
}
