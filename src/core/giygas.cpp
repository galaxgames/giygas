#include <giygas/giygas.hpp>
#include <giygas_internal/GLRenderer.hpp>

using namespace giygas;

Renderer *giygas::make_renderer(std::shared_ptr<Window> window) {
    return new GLRenderer(window);
}

