#pragma once
#include "Renderer.hpp"
#include "Context.hpp"

namespace giygas {
    using namespace std;

    GIYGAS_EXPORT Renderer *make_renderer(shared_ptr<Context> window);
    GIYGAS_EXPORT Renderer *make_renderer(
        shared_ptr<Context> window,
        RendererType type
    );
}