#pragma once
#include <giygas/Pipeline.hpp>
#include <giygas/PipelineCreateParameters.hpp>
#include <vulkan/vulkan.h>

namespace giygas {

    class VulkanRenderer;

    class VulkanPipeline : public Pipeline {

        VulkanRenderer *_renderer;
        VkPipelineLayout _layout;
        VkPipeline _handle;

        static VkShaderStageFlagBits shader_type_to_stage_flags(ShaderType type);

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
    };

}