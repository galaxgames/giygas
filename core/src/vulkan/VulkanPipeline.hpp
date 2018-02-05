#pragma once
#include <giygas/Pipeline.hpp>
#include <giygas/PipelineCreateParameters.hpp>
#include <vulkan/vulkan.h>

namespace giygas {

    class VulkanRenderer;

    class VulkanPipeline final : public Pipeline {

        VulkanRenderer *_renderer;
        VkPipelineLayout _layout;
        VkPipeline _handle;

        static VkShaderStageFlagBits shader_type_to_stage_flags(ShaderType type);

        static uint32_t count_total_attributes(
            const VertexAttributeLayout* layouts,
            size_t layout_count
        );

        static VkFormat get_attrib_format(size_t component_count);

    public:
        explicit VulkanPipeline(VulkanRenderer *renderer);
        VulkanPipeline(const VulkanPipeline &) = delete;
        VulkanPipeline &operator=(const VulkanPipeline &) = delete;
        VulkanPipeline(VulkanPipeline &&) = delete;
        VulkanPipeline &operator=(VulkanPipeline &&) = delete;
        ~VulkanPipeline() override;

        //
        // Pipeline implementation
        //

        void create(const PipelineCreateParameters &params) override;
        RendererType renderer_type() const override;


        //
        // VulkanPipeline implementation
        //

        VkPipeline handle() const;
    };

}