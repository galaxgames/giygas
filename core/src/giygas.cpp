#include <giygas/giygas.hpp>
#include "gl/GLRenderer.hpp"

using namespace giygas;

Renderer *giygas::make_renderer(std::shared_ptr<Context> window) {
    return new GLRenderer(window);
}

