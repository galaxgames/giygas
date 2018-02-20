#pragma once
#include "RendererType.hpp"
#include "UniformBuffer.hpp"
#include "Sampler.hpp"
#include "DescriptorPool.hpp"
#include "Texture.hpp"

namespace giygas {

    class DescriptorSetParameters {
    public:
        const DescriptorPool *pool;
        size_t uniform_buffer_count;
        const UniformBuffer * const * uniform_buffers;
        size_t sampler_count;
        const Sampler * const * samplers;
        const Texture * const * textures;
    };

    class DescriptorSet {
    public:
        virtual ~DescriptorSet() = default;
        virtual RendererType renderer_type() const = 0;
        virtual void create(const DescriptorSetParameters &params) = 0;
    };

}