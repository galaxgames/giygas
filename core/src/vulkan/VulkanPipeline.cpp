#include "VulkanPipeline.hpp"
#include "VulkanRenderer.hpp"
#include "VulkanShader.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanDescriptorSet.hpp"
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

    unique_ptr<VkVertexInputBindingDescription[]> input_bindings(
        new VkVertexInputBindingDescription[params.vertex_buffer_layout_count]
    );

    auto binding_count = static_cast<uint32_t>(params.vertex_buffer_layout_count);
    uint32_t total_attributes = count_total_attributes(
        params.vertex_buffer_layouts,
        params.vertex_buffer_layout_count
    );

    vector<VkVertexInputAttributeDescription> attribute_descriptions;
    attribute_descriptions.resize(total_attributes);

    size_t attrib_index = 0;
    for (
        uint32_t binding_index = 0;
        binding_index < params.vertex_buffer_layout_count;
        ++binding_index
    ) {
        const VertexAttributeLayout &layout = params.vertex_buffer_layouts[binding_index];
        VkVertexInputBindingDescription &binding = input_bindings[binding_index];
        binding = {};
        binding.binding = binding_index;
        binding.stride = static_cast<uint32_t>(layout.stride);
        binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        for (const LayoutAttribute &attrib : layout.attributes()) {
            VkVertexInputAttributeDescription &attrib_desc = attribute_descriptions[attrib_index];
            VkFormat format = get_attrib_format(attrib.component_count);
            assert(format != VK_FORMAT_UNDEFINED);

            attrib_desc = {};
            attrib_desc.binding = binding_index;
            attrib_desc.location = static_cast<uint32_t>(attrib_index);
            attrib_desc.format = format;
            attrib_desc.offset = static_cast<uint32_t>(attrib.offset);
            ++attrib_index;
        }
    }

    VkPipelineVertexInputStateCreateInfo vertex_input_info = {};
    vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_info.vertexBindingDescriptionCount = binding_count;
    vertex_input_info.pVertexBindingDescriptions = input_bindings.get();
    vertex_input_info.vertexAttributeDescriptionCount = total_attributes;
    vertex_input_info.pVertexAttributeDescriptions = attribute_descriptions.data();

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

    VkPipelineDepthStencilStateCreateInfo depth_stencil_state = {};
    depth_stencil_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depth_stencil_state.depthTestEnable = VK_FALSE;  // todo
    depth_stencil_state.depthWriteEnable = VK_FALSE;  // todo
    depth_stencil_state.depthCompareOp = VK_COMPARE_OP_NEVER;  // todo
    depth_stencil_state.depthBoundsTestEnable = VK_FALSE;
    depth_stencil_state.stencilTestEnable = VK_FALSE;  // todo
    depth_stencil_state.front = {};  // todo
    depth_stencil_state.back = {};  // todo
    depth_stencil_state.minDepthBounds = 0;  // todo
    depth_stencil_state.maxDepthBounds = 1;  // todo

    array<VkDynamicState, 0> dynamic_states = {
        //VK_DYNAMIC_STATE_VIEWPORT
    };

    VkPipelineDynamicStateCreateInfo dynamic_state = {};
    dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
    dynamic_state.pDynamicStates = dynamic_states.data();

    uint32_t push_constant_range_count = 0;
    array<VkPushConstantRange, 2> push_constant_ranges;
    if (params.vertex_push_constants.size > 0) {
        VkPushConstantRange &range = push_constant_ranges[push_constant_range_count];
        range = {};
        range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        range.size = static_cast<uint32_t>(params.vertex_push_constants.size);
        range.offset = static_cast<uint32_t>(params.vertex_push_constants.offset);
        ++push_constant_range_count;
    }
    if (params.fragment_push_constants.size > 0) {
        VkPushConstantRange &range = push_constant_ranges[push_constant_range_count];
        range = {};
        range.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        range.size = static_cast<uint32_t>(params.fragment_push_constants.size);
        range.offset = static_cast<uint32_t>(params.fragment_push_constants.offset);
        ++push_constant_range_count;
    }

    uint32_t set_layout_count;
    unique_ptr<VkDescriptorSetLayout[]> descriptor_set_layout_handles;
    if (params.descriptor_set != nullptr) {
        set_layout_count = 1;
        descriptor_set_layout_handles = unique_ptr<VkDescriptorSetLayout[]> (
            new VkDescriptorSetLayout[set_layout_count]
        );
        assert(params.descriptor_set->renderer_type() == RendererType::Vulkan);
        const auto *descriptor_set_impl = reinterpret_cast<const VulkanDescriptorSet *>(params.descriptor_set);
        descriptor_set_layout_handles[0] = descriptor_set_impl->layout();
    } else {
        set_layout_count = 0;
    }

    VkPipelineLayoutCreateInfo layout_info = {};
    layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layout_info.setLayoutCount = set_layout_count;
    layout_info.pSetLayouts = descriptor_set_layout_handles.get();
    layout_info.pushConstantRangeCount = push_constant_range_count;
    layout_info.pPushConstantRanges = push_constant_ranges.data();
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
    pipeline_info.pDepthStencilState = &depth_stencil_state;
    pipeline_info.pColorBlendState = &color_blend;
    pipeline_info.pDynamicState = nullptr; //&dynamic_state;
    pipeline_info.layout = _layout;
    pipeline_info.renderPass = renderpass->handle();
    pipeline_info.subpass = 0;
    pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
    pipeline_info.basePipelineIndex = -1;

    vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &_handle);
}

RendererType VulkanPipeline::renderer_type() const {
    return RendererType::Vulkan;
}

VkPipeline VulkanPipeline::handle() const {
    return _handle;
}

VkPipelineLayout VulkanPipeline::layout_handle() const {
    return _layout;
}

VkShaderStageFlagBits VulkanPipeline::shader_type_to_stage_flags(ShaderType type) {
    switch (type) {
        case ShaderType::Vertex:
            return VK_SHADER_STAGE_VERTEX_BIT;
        case ShaderType::Fragment:
            return VK_SHADER_STAGE_FRAGMENT_BIT;
    }
}

uint32_t VulkanPipeline::count_total_attributes(
    const VertexAttributeLayout *layouts,
    size_t layout_count
) {
    uint32_t count = 0;
    for (size_t i = 0; i < layout_count; ++i) {
        count += layouts[i].attributes().size();
    }
    return count;
}

VkFormat VulkanPipeline::get_attrib_format(size_t component_count) {
    switch (component_count) {
        case 1:
            return VK_FORMAT_R32_SFLOAT;
        case 2:
            return VK_FORMAT_R32G32_SFLOAT;
        case 3:
            return VK_FORMAT_R32G32B32_SFLOAT;
        case 4:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        default:
            return VK_FORMAT_UNDEFINED;
    }
}