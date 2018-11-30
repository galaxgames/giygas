#pragma once
#include <giygas/Framebuffer.hpp>
#include <giygas/Renderer.hpp>

namespace giygasutil {

    void create_basic_framebuffer(
        giygas::Framebuffer *framebuffer,
        const giygas::RenderTarget *rendertarget,
        const giygas::RenderPass *pass
    );

}
