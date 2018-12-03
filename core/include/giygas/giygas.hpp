#pragma once
#include "Renderer.hpp"
#include "Context.hpp"

namespace giygas {
    GIYGAS_EXPORT Renderer *make_renderer(Context *context);
    GIYGAS_EXPORT Renderer *make_renderer(Context *window, RendererType type);

    GIYGAS_EXPORT AttachmentPurpose attachment_purpose_from_texture_format(TextureFormat format);
    GIYGAS_EXPORT bool is_depth_format(TextureFormat format);
}
