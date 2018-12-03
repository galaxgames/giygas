#pragma once
#include <giygas/Pipeline.hpp>
#include <giygas/Framebuffer.hpp>
#include <giygas/VertexBuffer.hpp>
#include <giygas/IndexBuffer.hpp>
#include "IndexRange.hpp"
#include "UniformBuffer.hpp"
#include "DescriptorSet.hpp"

namespace giygas {

    class DepthStencilClearValue {
    public:
        float depth;
        uint32_t stencil;
    };

    class ClearValue {
    public:
        AttachmentPurpose purpose;
        union {
            Vector4 color_value;
            DepthStencilClearValue depth_stencil;
        };
    };

    class PassExecutionInfo {
    public:
        const RenderPass *pass;
        const Framebuffer *framebuffer;
        uint32_t clear_value_count;
        const ClearValue *clear_values;
    };

    class PushConstants {
    public:
        PushConstantsRange range;
        const uint8_t *data;
    };

    class DrawInfo {
    public:
        const Pipeline *pipeline;
        uint32_t vertex_buffer_count;
        const VertexBuffer * const *vertex_buffers;
        const GenericIndexBuffer *index_buffer;
        const DescriptorSet *descriptor_set;
        IndexRange index_range;
        PushConstants vertex_push_constants;
        PushConstants fragment_push_constants;
    };

    class PassSubmissionInfo {
    public:
        PassExecutionInfo pass_info;
        uint32_t draw_count;
        const DrawInfo *draws;
    };

}
