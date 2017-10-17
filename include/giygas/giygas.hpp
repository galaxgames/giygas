#pragma once
#include "Renderer.hpp"
#include "Window.hpp"

namespace giygas {
    Renderer *make_renderer(std::shared_ptr<Window> window);
}