#pragma once
#include "RendererType.hpp"
#include "RenderPassCreateParameters.hpp"

namespace giygas {

    class RenderPass {
    public:
        virtual ~RenderPass() = 0;
        virtual RendererType renderer_type() const = 0;
        virtual void create(const RenderPassCreateParameters &params) = 0;
    };

}