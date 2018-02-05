#pragma once
#include <giygas/export.h>
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Material.hpp"
#include "AttachmentType.hpp"
#include "Surface.hpp"
#include "PipelineOptions.hpp"
#include "SamplerOptions.hpp"
#include "Pipeline.hpp"
#include "Framebuffer.hpp"
#include "RenderBuffer.hpp"

namespace giygas {

    class GIYGAS_EXPORT Renderer {
    public:
        virtual ~Renderer() = default;
        virtual void initialize() = 0;

        virtual VertexBuffer *make_vertex_buffer() = 0;
        virtual IndexBuffer<uint32_t> *make_index_buffer_32() = 0;
        virtual IndexBuffer<uint16_t > *make_index_buffer_16() = 0;
        virtual IndexBuffer<uint8_t> *make_index_buffer_8() = 0;
        virtual Material *make_material() = 0;
        virtual Shader *make_shader() = 0;
        virtual Texture *make_texture(SamplerOptions options) = 0;
        virtual Framebuffer *make_framebuffer() = 0;
        virtual RenderBuffer *make_renderbuffer() = 0;
        virtual Pipeline *make_pipeline() = 0;
        virtual Surface *main_surface() = 0;

        virtual void present() = 0;
    };
}


