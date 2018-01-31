#pragma once
#include <giygas/RenderPass.hpp>
#include <giygas/RenderPassCreateParameters.hpp>
#include <vulkan/vulkan.h>

namespace giygas {

    class VulkanRenderer;

    class VulkanRenderPass : public RenderPass {

        VulkanRenderer *_renderer;
        VkRenderPass _handle;

    public:
        explicit VulkanRenderPass(VulkanRenderer *renderer);
        VulkanRenderPass(const VulkanRenderPass &) = delete;
        VulkanRenderPass &operator=(const VulkanRenderPass &) = delete;
        VulkanRenderPass(VulkanRenderPass &&) = delete;
        VulkanRenderPass &operator=(VulkanRenderPass &&) = delete;
        ~VulkanRenderPass() override;

        //
        // RenderPass implementation
        //

        void create(const RenderPassCreateParameters &params) override;

        //
        // VulkanRenderPass implementation
        //

        VkRenderPass handle() const;

    };

}