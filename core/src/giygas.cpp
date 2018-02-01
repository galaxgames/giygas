#include <giygas/giygas.hpp>

#if GIYGAS_WITH_VULKAN
#include "vulkan/VulkanRenderer.hpp"
#endif
#if GIYGAS_WITH_OPENGL
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
#if VULKAN_WITH_OPENGL
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
        case RendererType::Vulkan: {
            auto *vulkan_context = static_cast<VulkanContext *>(
                context->cast_to_specific(type)
            );
            if (vulkan_context != nullptr) {
                return new VulkanRenderer(vulkan_context);
            }
            break;
        }
        default:
            break;
    }
    return nullptr;
}

