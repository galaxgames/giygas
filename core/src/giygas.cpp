#include <giygas/giygas.hpp>
#include <giygas/config.hpp>

#ifdef GIYGAS_WITH_VULKAN
#include "vulkan/VulkanRenderer.hpp"
#endif
#ifdef GIYGAS_WITH_OPENGL
#include <giygas/GLContext.hpp>
#endif

using namespace giygas;

Renderer *giygas::make_renderer(Context *context) {
    Renderer *renderer = make_renderer(context, RendererType::Vulkan);
    if (renderer == nullptr) {
        renderer = make_renderer(context, RendererType::OpenGL);
    }
    return renderer;
}

Renderer* giygas::make_renderer(Context *context, RendererType type) {
    switch (type) {
#ifdef GIYGAS_WITH_OPENGL
        case RendererType::OpenGL: {
            auto *gl_context = static_cast<GLContext *>(
                context->cast_to_specific(type)
            );
            if (gl_context != nullptr) {
                //return new GLRenderer(context);
            }
            break;
        }
#endif
#ifdef GIYGAS_WITH_VULKAN
        case RendererType::Vulkan: {
            auto *vulkan_context = static_cast<VulkanContext *>(
                context->cast_to_specific(type)
            );
            if (vulkan_context != nullptr) {
                return new VulkanRenderer(vulkan_context);
            }
            break;
        }
#endif
        default:
            break;
    }
    return nullptr;
}

AttachmentPurpose giygas::attachment_purpose_from_texture_format(TextureFormat format) {
    switch (format) {
        case TextureFormat::RGB:
        case TextureFormat::RGBA:
            return AttachmentPurpose::Color;
        case TextureFormat::Depth16:
        case TextureFormat::Depth24:
        case TextureFormat::Depth32:
        case TextureFormat::Depth32Float:
            return AttachmentPurpose::DepthStencil;
    }
    assert(false);
    return AttachmentPurpose::Unknown;
}

bool giygas::is_depth_format(TextureFormat format) {
    return (
        format == TextureFormat::Depth16 ||
        format == TextureFormat::Depth24 ||
        format == TextureFormat::Depth32 ||
        format == TextureFormat::Depth32Float
    );
}
