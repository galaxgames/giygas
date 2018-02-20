#pragma once
#include <cstddef>
#include <cstdint>
#include "RendererType.hpp"

namespace giygas {

    class UniformBuffer {
    public:
        virtual ~UniformBuffer() = default;
        virtual RendererType renderer_type() const = 0;
        virtual void set_data(size_t offset, const uint8_t *data, size_t size) = 0;
    };

}