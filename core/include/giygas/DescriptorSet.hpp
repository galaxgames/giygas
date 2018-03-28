#pragma once
#include "RendererType.hpp"
#include "UniformBuffer.hpp"
#include "Sampler.hpp"
#include "DescriptorPool.hpp"
#include "Texture.hpp"

namespace giygas {


    // TODO: Make this an enum class
    enum ShaderStage {
        GIYGAS_SHADER_STAGE_NONE = 0,
        GIYGAS_SHADER_STAGE_VERTEX = 1 << 0,
        GIYGAS_SHADER_STAGE_FRAGMENT = 1 << 1
    };

    class UniformBufferDescriptorSlot {
    public:
        uint32_t binding_index;
        ShaderStage stages;
    };

    class SamplerDescriptorSlot {
    public:
        uint32_t binding_index;
        ShaderStage stages;
        const Sampler *immutable_sampler;
    };

    class UniformBufferDescriptorBinding {
    public:
        uint32_t binding_index;
        const UniformBuffer *buffer;
    };

    class SamplerDescriptorBinding {
    public:
        uint32_t binding_index;
        const Sampler *sampler;
        const Texture *texture;
    };

    class DescriptorSetCreateParameters {
    public:
        const DescriptorPool *pool;
        uint32_t uniform_buffer_count;
        const UniformBufferDescriptorSlot *uniform_buffer_slots;
        uint32_t sampler_count;
        const SamplerDescriptorSlot *sampler_slots;
    };

    class DescriptorSetUpdateParameters {
    public:
        uint32_t uniform_buffer_count;
        const UniformBufferDescriptorBinding *uniform_buffer_bindings;
        uint32_t sampler_count;
        const SamplerDescriptorBinding *sampler_bindings;
    };

    class DescriptorSet {
    public:
        virtual ~DescriptorSet() = default;
        virtual RendererType renderer_type() const = 0;
        virtual bool is_created() const = 0;
        virtual bool has_descriptors() const = 0;
        virtual void create(const DescriptorSetCreateParameters &params) = 0;
        virtual void update(const DescriptorSetUpdateParameters &params) = 0;
    };

}