#pragma once
#include <cstddef>
#include <cstdint>
#include "AttachmentPurpose.hpp"
#include "RendererType.hpp"
#include "RenderTarget.hpp"
#include "RenderPass.hpp"

namespace giygas {

    class FramebufferAttachment  {
    public:
        AttachmentPurpose purpose;
        const RenderTarget *target;
    };

    class FramebufferCreateParameters {
    public:
        const RenderPass *pass;
        const FramebufferAttachment *attachments;
        size_t attachment_count;
        uint32_t width;
        uint32_t height;
    };

    class Framebuffer {
    public:
        virtual ~Framebuffer() = default;
        virtual RendererType renderer_type() const = 0;
        virtual void create(const FramebufferCreateParameters &params) = 0;
        virtual bool is_valid() const = 0;
        virtual uint32_t width() const = 0;
        virtual uint32_t height() const = 0;
        virtual size_t attachment_count() const = 0;
        virtual const AttachmentPurpose *attachment_purposes() const = 0;
    };

}