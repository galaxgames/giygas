#pragma once
#include "Surface.hpp"
#include "RenderBuffer.hpp"

namespace giygas {
    class FrameBufferSurface : public Surface {
    public:
        virtual ~FrameBufferSurface() = default;
        virtual void attach_texture(
            Texture *texture,
            AttachmentType attachment_type
        ) = 0;
        virtual void attach_renderbuffer(
            RenderBuffer *renderbuffer,
            AttachmentType attachment_type
        ) = 0;
    };
}