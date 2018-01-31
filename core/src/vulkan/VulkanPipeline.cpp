#include "VulkanPipeline.hpp"
#include "VulkanRenderer.hpp"
#include "VulkanShader.hpp"
#include "VulkanRenderPass.hpp"
#include <cassert>

using namespace giygas;
using namespace std;

VulkanPipeline::VulkanPipeline(VulkanRenderer *renderer) {
    _renderer = renderer;
    _layout = VK_NULL_HANDLE;
    _handle = VK_NULL_HANDLE;
}

VulkanPipeline::~VulkanPipeline() {
    VkDevice device = _renderer->device();
    vkDestroyPipeline(device, _handle, nullptr);
    vkDestroyPipelineLayout(device, _layout, nullptr);
}

void VulkanPipeline::create(const PipelineCreateParameters &params) {
    VkDevice device = _renderer->device();

    VkPipelineVertexInputStateCreateInfo vertex_input_info = {};
    vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_info.vertexBindingDescriptionCount = 0;
    vertex_input_info.pVertexBindingDescriptions = nullptr;
    vertex_input_info.vertexAttributeDescriptionCount = 0;
    vertex_input_info.pVertexAttributeDescriptions = nullptr;

    VkPipelineInputAssemblyStateCreateInfo input_assembly_info = {};
    input_assembly_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    input_assembly_info.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport = {};
    viewport.x = params.viewport.x;
    viewport.y = params.viewport.y;
    viewport.width = params.viewport.width;
    viewport.height = params.viewport.height;
    viewport.minDepth = params.viewport.min_depth;
    viewport.maxDepth = params.viewport.max_depth;

    VkRect2D scissor = {};
    scissor.offset = { params.scissor.x, params.scissor.y };
    scissor.extent = { params.scissor.width, params.scissor.height };

    VkPipelineViewportStateCreateInfo viewport_state = {};
    viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state.viewportCount = 1;
    viewport_state.pViewports = &viewport;
    viewport_state.scissorCount = 1;
    viewport_state.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0;
    rasterizer.depthBiasClamp = 0;
    rasterizer.depthBiasSlopeFactor = 0;


    VkPipelineMultisampleStateCreateInfo multisample = {};
    multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample.sampleShadingEnable = VK_FALSE;
    multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisample.minSampleShading = 1;
    multisample.pSampleMask = nullptr;
    multisample.alphaToCoverageEnable = VK_FALSE;
    multisample.alphaToOneEnable = VK_FALSE;


    // COLOR BLENDING CHEATSHEET
    // if (blendEnable) {
    //     finalColor.rgb = (srcColorBlendFactor * newColor.rgb) <colorBlendOp> (dstColorBlendFactor * oldColor.rgb);
    //     finalColor.a = (srcAlphaBlendFactor * newColor.a) <alphaBlendOp> (dstAlphaBlendFactor * oldColor.a);
    // } else {
    //     finalColor = newColor;
    // }
    //
    // finalColor = finalColor & colorWriteMask;

    VkPipelineColorBlendAttachmentState blend_attachment = {};
    blend_attachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT
        | VK_COLOR_COMPONENT_G_BIT
        | VK_COLOR_COMPONENT_B_BIT;
    blend_attachment.blendEnable = VK_FALSE;
    blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
    blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo color_blend = {};
    color_blend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blend.logicOpEnable = VK_FALSE;
    color_blend.logicOp = VK_LOGIC_OP_COPY;
    color_blend.attachmentCount = 1;
    color_blend.pAttachments = &blend_attachment;
    color_blend.blendConstants[0] = 0;
    color_blend.blendConstants[1] = 0;
    color_blend.blendConstants[2] = 0;
    color_blend.blendConstants[3] = 0;

    array<VkDynamicState, 1> dynamic_states = {
        VK_DYNAMIC_STATE_VIEWPORT
    };

    VkPipelineDynamicStateCreateInfo dynamic_state = {};
    dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
    dynamic_state.pDynamicStates = dynamic_states.data();

    VkPipelineLayoutCreateInfo layout_info = {};
    layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layout_info.setLayoutCount = 0;
    layout_info.pSetLayouts = nullptr;
    layout_info.pushConstantRangeCount = 0;
    layout_info.pPushConstantRanges = nullptr;
    if (vkCreatePipelineLayout(device, &layout_info, nullptr, &_layout) != VK_SUCCESS) {
        return;
    }

    unique_ptr<VkPipelineShaderStageCreateInfo[]> shader_stages(
        new VkPipelineShaderStageCreateInfo[params.shader_count]{}
    );
    for (size_t i = 0; i < params.shader_count; ++i) {
        VkPipelineShaderStageCreateInfo &stage = shader_stages[i];
        const Shader *public_shader = params.shaders[i];
        assert(public_shader->renderer_type() == RendererType::Vulkan);
        const auto *shader = reinterpret_cast<const VulkanShader *>(public_shader);
        stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stage.stage = shader_type_to_stage_flags(shader->shader_type());
        stage.module = shader->module();
        stage.pName = "main";
    }

    const RenderPass *public_renderpass = params.render_pass;
    assert(public_renderpass->renderer_type() == RendererType::Vulkan);
    const auto *renderpass = reinterpret_cast<const VulkanRenderPass *>(public_renderpass);

    VkGraphicsPipelineCreateInfo pipeline_info = {};
    pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_info.stageCount = static_cast<uint32_t>(params.shader_count);
    pipeline_info.pStages = shader_stages.get();
    pipeline_info.pVertexInputState = &vertex_input_info;
    pipeline_info.pInputAssemblyState = &input_assembly_info;
    pipeline_info.pViewportState = &viewport_state;
    pipeline_info.pRasterizationState = &rasterizer;
    pipeline_info.pMultisampleState = &multisample;
    pipeline_info.pDepthStencilState = nullptr;  // TODO
    pipeline_info.pColorBlendState = &color_blend;
    pipeline_info.pDynamicState = &dynamic_state;
    pipeline_info.layout = _layout;
    pipeline_info.renderPass = renderpass->handle();
    pipeline_info.subpass = 0;
    pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
    pipeline_info.basePipelineIndex = -1;

    vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &_handle);
}

VkShaderStageFlagBits VulkanPipeline::shader_type_to_stage_flags(ShaderType type) {
    switch (type) {
        case ShaderType::Vertex:
            return VK_SHADER_STAGE_VERTEX_BIT;
        case ShaderType::Fragment:
            return VK_SHADER_STAGE_FRAGMENT_BIT;
    }
}