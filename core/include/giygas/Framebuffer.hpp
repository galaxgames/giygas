#pragma once
#include "AttachmentType.hpp"
#include "RendererType.hpp"
#include <cstddef>
#include <cstdint>

namespace giygas {

    class Texture;
    class RenderBuffer;
    class RenderPass;

    class FramebufferAttachment  {
    public:
        AttachmentType type;
        AttachmentPurpose purpose;
        union {
            const Texture *texture;
            const RenderBuffer *render_buffer;
        };
    };

    class FramebufferCreateParameters {
    public:
        const FramebufferAttachment *attachments;
        size_t attachment_count;
        const RenderPass *pass;
        uint32_t width;
        uint32_t height;
    };

    class Framebuffer {
    public:
        virtual ~Framebuffer() = default;
        virtual RendererType renderer_type() const = 0;
        virtual uint32_t width() const = 0;
        virtual uint32_t height() const = 0;
    };

}