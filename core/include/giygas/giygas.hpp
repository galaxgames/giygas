#pragma once
#include "Renderer.hpp"
#include "Context.hpp"

namespace giygas {
    GIYGAS_EXPORT Renderer *make_renderer(std::shared_ptr<Context> window);
}