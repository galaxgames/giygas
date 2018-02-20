#pragma once
#include <giygas/DescriptorPool.hpp>
#include <vulkan/vulkan.h>

namespace giygas {

    class VulkanRenderer;

    class VulkanDescriptorPool final : public DescriptorPool {

        VulkanRenderer *_renderer;
        VkDescriptorPool _handle;

    public:
        explicit VulkanDescriptorPool(VulkanRenderer *renderer);
        ~VulkanDescriptorPool() override;

        //
        // DescriptorPool implementation
        //

        RendererType renderer_type() const override;
        void create(const DescriptorPoolParameters &params) override;


        //
        // VulkanDescriptorPool implementation
        //
        VkDescriptorPool handle() const;

    };

}