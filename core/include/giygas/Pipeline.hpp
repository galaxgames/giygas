#pragma once
#include "PipelineCreateParameters.hpp"

namespace giygas {

    class Pipeline {
    public:
        virtual ~Pipeline() = default;
        virtual RendererType renderer_type() const = 0;
        virtual void create(const PipelineCreateParameters &params) = 0;
        virtual uint8_t descriptor_set_count() const = 0;
        virtual bool is_descriptor_set_compatible(const DescriptorSet *descriptor_set) const = 0;
    };

}