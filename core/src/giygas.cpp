#include <giygas/giygas.hpp>
#include "gl/GLRenderer.hpp"
#include "vulkan/VulkanRenderer.hpp"

using namespace giygas;

Renderer *giygas::make_renderer(std::shared_ptr<Context> window) {
    Renderer *renderer = make_renderer(window, RendererType::Vulkan);
    if (renderer == nullptr) {
        renderer = make_renderer(window, RendererType::OpenGL);
    }
    return renderer;
}

Renderer* giygas::make_renderer(shared_ptr<Context> window, RendererType type) {
    switch (type) {
        case RendererType::OpenGL:
            return new GLRenderer(window);
        case RendererType::Vulkan:
            return new VulkanRenderer(window);
    }
    return nullptr;
}

