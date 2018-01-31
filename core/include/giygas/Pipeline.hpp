#pragma once
#include "PipelineCreateParameters.hpp"

namespace giygas {

    class Pipeline {
    public:
        virtual ~Pipeline() = 0;
        virtual void create(const PipelineCreateParameters &params) = 0;
    };

}