#pragma once
#include <cstdint>

namespace giygas {

    class RenderPassAttachmentParameters {
    public:
        uint32_t api_format;
    };

    class RenderPassCreateParameters {
    public:
        RenderPassAttachmentParameters color_attachment;
        RenderPassAttachmentParameters stencil_attachment;
        RenderPassAttachmentParameters depth_attachment;
    };

}