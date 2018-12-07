#pragma once
#include <giygas/export.h>
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "AttachmentPurpose.hpp"
#include "PipelineOptions.hpp"
#include "SamplerParameters.hpp"
#include "Pipeline.hpp"
#include "Framebuffer.hpp"
#include "UniformBuffer.hpp"
#include "Sampler.hpp"
#include "RenderPass.hpp"
#include "submission.hpp"

namespace giygas {

    class GIYGAS_EXPORT Renderer {
    public:
        virtual ~Renderer() = default;
        virtual void initialize() = 0;
        virtual RendererType renderer_type() const = 0;
        virtual VertexBuffer *make_vertex_buffer(VertexBufferCreateFlags flags) = 0;
        virtual IndexBuffer<uint32_t> *make_index_buffer_32(IndexBufferCreateFlags flags) = 0;
        virtual IndexBuffer<uint16_t > *make_index_buffer_16(IndexBufferCreateFlags flags) = 0;
        virtual IndexBuffer<uint8_t> *make_index_buffer_8(IndexBufferCreateFlags flags) = 0;
        virtual UniformBuffer *make_uniform_buffer() = 0;
        virtual Shader *make_shader() = 0;
        virtual Texture *make_texture() = 0;
        virtual Sampler *make_sampler() = 0;
        virtual DescriptorPool *make_descriptor_pool() = 0;
        virtual DescriptorSet *make_descriptor_set() = 0;
        virtual RenderPass *make_render_pass() = 0;
        virtual Framebuffer *make_framebuffer() = 0;
        virtual Pipeline *make_pipeline() = 0;

        virtual const RenderTarget *swapchain() const = 0;

        //virtual uint32_t get_api_texture_format(TextureFormat format) const = 0;

        virtual void submit(const PassSubmissionInfo *passes, uint32_t pass_count) = 0;
    };
}


