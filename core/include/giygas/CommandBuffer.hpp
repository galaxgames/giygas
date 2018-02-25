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
        size_t clear_value_count;
        const ClearValue *clear_values;
    };

    class DrawInfo {
    public:
        const Pipeline *pipeline;
        size_t vertex_buffer_count;
        const VertexBuffer * const *vertex_buffers;
        const GenericIndexBuffer *index_buffer;
        const DescriptorSet *descriptor_set;
        IndexRange index_range;
        size_t push_constants_offset;
        size_t push_constants_size;
        const uint8_t *push_constants;
    };

    class SingleBufferPassInfo {
    public:
        PassExecutionInfo pass_info;
        size_t draw_count;
        const DrawInfo *draws;
    };

    class CommandBuffer {
    public:
        virtual ~CommandBuffer() = default;
        virtual RendererType renderer_type() const = 0;
        virtual void record_pass(const SingleBufferPassInfo &info) = 0;
    };

}