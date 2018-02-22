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

    class DrawInfo {
    public:
        const Pipeline *pipeline;
        const Framebuffer *framebuffer;
        size_t vertex_buffer_count;
        const VertexBuffer * const *vertex_buffers;
        const GenericIndexBuffer *index_buffer;
        const DescriptorSet *descriptor_set;
        IndexRange index_range;
        size_t push_constants_offset;
        size_t push_constants_size;
        const uint8_t *push_constants;
        const ClearValue *clear_values;
    };

    class CommandBuffer {
    public:
        virtual ~CommandBuffer() = default;
        virtual RendererType renderer_type() const = 0;
        virtual void record(const DrawInfo &info) = 0;
    };

}