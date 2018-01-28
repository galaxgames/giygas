#pragma once
#include "Renderer.hpp"
#include "Context.hpp"

namespace giygas {
    using namespace std;

    GIYGAS_EXPORT Renderer *make_renderer(Context *context);
    GIYGAS_EXPORT Renderer *make_renderer(Context *window, RendererType type);
}