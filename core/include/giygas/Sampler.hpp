#pragma once
#include "SamplerParameters.hpp"
#include "RendererType.hpp"

namespace giygas {

    class Sampler {
    public:
        virtual ~Sampler() = default;
        virtual RendererType renderer_type() const = 0;
        virtual void create(const SamplerParameters &params) = 0;
    };

}