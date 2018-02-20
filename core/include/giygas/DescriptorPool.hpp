#pragma once
#include <cstdint>
#include "RendererType.hpp"

namespace giygas {

    class DescriptorPoolParameters {
    public:
        uint32_t max_sets;
        uint32_t uniform_buffer_descriptors;
        uint32_t sampler_descriptors;
    };

    class DescriptorPool {
    public:
        virtual ~DescriptorPool() = default;
        virtual RendererType renderer_type() const = 0;
        virtual void create(const DescriptorPoolParameters &params) = 0;
    };

}