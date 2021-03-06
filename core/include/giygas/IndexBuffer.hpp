#pragma once
#include <cstddef>
#include <giygas/export.h>
#include <cstdint>
#include "RendererType.hpp"

namespace giygas {

    //
    // TODO: Get rid of these classes. Combine them with VertexBuffer, so we can possibly
    // store both vertex and index data in the same buffer. This is recomended by gpu driver
    // developers for performance.
    //
    // Note: Above TODO might not be applicable for Vulkan. Lots of unknowns here.
    //

    enum IndexBufferCreateFlags {
        IndexBufferCreateFlag_None     = 0,
        IndexBufferCreateFlag_Writable = 1 << 0
    };

    class GenericIndexBuffer {
    public:
        virtual ~GenericIndexBuffer() = default;
        virtual RendererType renderer_type() const = 0;
        virtual const void *cast_to_specific() const = 0;
    };

    template <typename T>
    class GIYGAS_EXPORT IndexBuffer : public GenericIndexBuffer {
    public:
        ~IndexBuffer() override = default;
        virtual void set(uint32_t offset, const T *indices, uint32_t count) = 0;
        virtual size_t count() const = 0;
    };

    typedef IndexBuffer<uint32_t> IndexBuffer32;
    typedef IndexBuffer<uint16_t> IndexBuffer16;
    typedef IndexBuffer<uint8_t> IndexBuffer8;
}
