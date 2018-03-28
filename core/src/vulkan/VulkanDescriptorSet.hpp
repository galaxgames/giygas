#pragma once
#include <giygas/DescriptorSet.hpp>
#include <vulkan/vulkan.h>
#include "VulkanDescriptorPool.hpp"

namespace giygas {

    class VulkanRenderer;

    class VulkanDescriptorSet final : public DescriptorSet {

        VulkanRenderer *_renderer;
        VkDescriptorSetLayout _layout;
        VkDescriptorSet _handle;
        VkDescriptorPool _pool;
        uint32_t _uniform_buffer_count;
        uint32_t _sampler_count;
        bool _has_descriptors;

        VkShaderStageFlags translate_shader_stages(ShaderStage stages) const;

    public:
        VulkanDescriptorSet(VulkanRenderer *renderer);
        ~VulkanDescriptorSet() override;

        //
        // DescriptorSet implementation
        //

        RendererType renderer_type() const override;
        bool is_created() const override;
        bool has_descriptors() const override;
        void create(const DescriptorSetCreateParameters &params) override;
        void update(const DescriptorSetUpdateParameters &params) override;

        //
        // VulkanDescriptorSet implementation
        //

        void create(const VulkanDescriptorPool *pool, const DescriptorSetCreateParameters &params);
        VkDescriptorSetLayout layout() const;
        VkDescriptorSet handle() const;


    };

}