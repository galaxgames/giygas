#pragma once
#include <cstddef>
#include "AttachmentPurpose.hpp"
#include "RenderTarget.hpp"

namespace giygas {

    class RenderPassAttachment  {
    public:
        AttachmentPurpose purpose;
        const RenderTarget *target;
    };

    class RenderPassCreateParameters {
    public:
        uint32_t attachment_count;
        const RenderPassAttachment *attachments;
    };

    class RenderPass {
    public:
        virtual ~RenderPass() = default;
        virtual RendererType renderer_type() const = 0;
        virtual void create(const RenderPassCreateParameters &params) = 0;
        virtual bool is_valid() const = 0;
        virtual uint32_t attachment_count() const = 0;
        virtual const AttachmentPurpose *attachment_purposes() const = 0;
    };

}
