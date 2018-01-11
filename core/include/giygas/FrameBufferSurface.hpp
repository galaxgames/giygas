#pragma once
#include "Surface.hpp"
#include "RenderBuffer.hpp"

namespace giygas {
    class FrameBufferSurface : public Surface {
    public:
        virtual ~FrameBufferSurface() = default;
        virtual void attach_texture(
            Texture *texture,
            SurfaceBufferType attachment_type
        ) = 0;
        virtual void attach_renderbuffer(
            RenderBuffer *renderbuffer,
            SurfaceBufferType attachment_type
        ) = 0;
    };
}