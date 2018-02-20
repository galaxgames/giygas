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

    public:
        VulkanDescriptorSet(VulkanRenderer *renderer);
        ~VulkanDescriptorSet() override;

        //
        // DescriptorSet implementation
        //

        RendererType renderer_type() const override;
        void create(const DescriptorSetParameters &params) override;


        //
        // VulkanDescriptorSet implementation
        //

        void create(const VulkanDescriptorPool *pool, const DescriptorSetParameters &params);
        VkDescriptorSetLayout layout() const;
        VkDescriptorSet handle() const;


    };

}