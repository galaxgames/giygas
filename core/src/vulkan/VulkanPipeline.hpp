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
        VkDescriptorSetLayout _descriptor_set_layout;

        static VkShaderStageFlagBits shader_type_to_stage_flags(ShaderType type);

        static uint32_t count_total_attributes(
            const VertexAttributeLayout* layouts,
            size_t layout_count
        );

        static VkFormat get_attrib_format(size_t component_count);
        static VkBlendOp translate_blend_op(BlendOperation op);
        static VkBlendFactor translate_blend_factor(BlendFactor factor);

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

        RendererType renderer_type() const override;
        void create(const PipelineCreateParameters &params) override;
        uint8_t descriptor_set_count() const override;
        bool is_descriptor_set_compatible(const DescriptorSet *descriptor_set) const override;


        //
        // VulkanPipeline implementation
        //

        VkPipeline handle() const;
        VkPipelineLayout layout_handle() const;
    };

}