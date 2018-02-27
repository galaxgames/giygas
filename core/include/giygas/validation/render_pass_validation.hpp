#pragma once
#include <giygas/export.h>
#include <giygas/RenderPass.hpp>

namespace giygas {
namespace validation {

    GIYGAS_EXPORT bool validate_render_pass_create(
        const RenderPass *target,
        const RenderPassCreateParameters &params
    );

}}
