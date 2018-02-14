#pragma once
#include <giygas/Pipeline.hpp>
#include <giygas/Framebuffer.hpp>
#include <giygas/VertexBuffer.hpp>
#include <giygas/IndexBuffer.hpp>
#include <giygas/Material.hpp>
#include "IndexRange.hpp"

namespace giygas {

    class DrawInfo {
    public:
        const Pipeline *pipeline;
        const RenderPass *pass;
        const Framebuffer *framebuffer;
        const VertexBuffer * const *vertex_buffers;
        size_t vertex_buffer_count;
        const GenericIndexBuffer *index_buffer;
        const Material *material;
        IndexRange index_range;
    };


    class CommandBufferImpl;

    class CommandBuffer final {
        CommandBufferImpl *_impl;
    public:
        CommandBuffer();
        explicit CommandBuffer(CommandBufferImpl *impl);
        CommandBuffer(const CommandBuffer &) = delete;
        CommandBuffer &operator=(const CommandBuffer &) = delete;
        CommandBuffer(CommandBuffer &&) noexcept;
        CommandBuffer &operator=(CommandBuffer &&) noexcept;
        ~CommandBuffer();
        void record(const DrawInfo &info);
        const CommandBufferImpl *impl() const;
    };

}