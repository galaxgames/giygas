#pragma once
#include "PipelineCreateParameters.hpp"

namespace giygas {

    class Pipeline {
    public:
        virtual ~Pipeline() = default;
        virtual void create(const PipelineCreateParameters &params) = 0;
        virtual RendererType renderer_type() const = 0;
    };

}