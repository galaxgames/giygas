#pragma once
#include <giygas/RenderPass.hpp>
#include <vulkan/vulkan.h>
#include <memory>
#include "VulkanRenderTarget.hpp"

namespace giygas {
    using namespace std;

    class VulkanRenderer;

    class VulkanRenderPass final : public RenderPass {

        VulkanRenderer *_renderer;
        VkRenderPass _handle;
        unique_ptr<AttachmentPurpose[]> _purposes;

        static void set_description_from_attachmnent_params(
            VkAttachmentDescription &description,
            const VulkanRenderTarget *target
        );

    public:
        explicit VulkanRenderPass(VulkanRenderer *renderer);
        ~VulkanRenderPass() override;

        //
        // RenderPass implementation
        //

        RendererType renderer_type() const override;
        void create(const RenderPassCreateParameters &params) override;


        //
        // VulkanRenderPass implementation
        //

        VkRenderPass handle() const;
        const AttachmentPurpose *purposes() const;

    };

}