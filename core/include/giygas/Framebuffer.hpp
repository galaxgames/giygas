#pragma once
#include <cstddef>
#include <cstdint>
#include "AttachmentType.hpp"
#include "RendererType.hpp"
#include "RenderTarget.hpp"

namespace giygas {

    class Texture;
    class RenderBuffer;
    class RenderPass;

    class FramebufferAttachment  {
    public:
        AttachmentPurpose purpose;
        const RenderTarget *target;
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