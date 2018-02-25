#pragma once
#include <giygas/Framebuffer.hpp>
#include <giygas/Renderer.hpp>

namespace giygasutil {
namespace util {
    using namespace giygas;

    template <typename PtrT>
    PtrT *create_swapchain_framebuffers(Renderer *renderer, const RenderPass *pass);

}}