#pragma once

#include "Pipeline.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Material.hpp"
#include "IndexRange.hpp"
#include "Framebuffer.hpp"

namespace giygas {

    class DrawInfo {
    public:
        const Pipeline *pipeline;
        const RenderPass *pass;
        const Framebuffer *framebuffer;
        const VertexBuffer * const *vertex_buffers;
        size_t vertex_buffer_count;
        const GenericIndexBuffer *index_buffer;
        const Shader *shader;
        const Material *material;
        IndexRange index_range;
    };

    class CommandPool {
    public:
        virtual ~CommandPool() = default;

        virtual void draw(const DrawInfo &info) = 0;

    };


}