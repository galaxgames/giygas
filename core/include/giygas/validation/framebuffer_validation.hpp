#pragma once
#include <giygas/Framebuffer.hpp>

namespace giygas {
namespace validation {

    bool validate_framebuffer_create(
        const Framebuffer *target,
        const FramebufferCreateParameters &params
    );

}}
