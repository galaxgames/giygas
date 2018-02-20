#pragma once
#include <giygas/Pipeline.hpp>
#include <giygas/Framebuffer.hpp>
#include <giygas/VertexBuffer.hpp>
#include <giygas/IndexBuffer.hpp>
#include "IndexRange.hpp"
#include "UniformBuffer.hpp"
#include "DescriptorSet.hpp"

namespace giygas {

    class DrawInfo {
    public:
        const Pipeline *pipeline;
        const RenderPass *pass;
        const Framebuffer *framebuffer;
        size_t vertex_buffer_count;
        const VertexBuffer * const *vertex_buffers;
        const GenericIndexBuffer *index_buffer;
        const DescriptorSet *descriptor_set;
        IndexRange index_range;
        size_t push_constants_offset;
        size_t push_constants_size;
        const uint8_t *push_constants;
    };

    class CommandBuffer {
    public:
        virtual ~CommandBuffer() = default;
        virtual RendererType renderer_type() const = 0;
        virtual void record(const DrawInfo &info) = 0;
    };

}