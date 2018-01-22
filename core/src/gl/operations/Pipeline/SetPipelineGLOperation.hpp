#pragma once

#include <giygas/PipelineOptions.hpp>
#include "../../GLOperation.hpp"

namespace giygas {

    class SetPipelineGLOperation : public GLOperation {
        PipelineOptions _options;

        static GLenum depth_function_to_enum(DepthFunction function);
    public:
        SetPipelineGLOperation();
        void set(PipelineOptions options);
        void execute(GL *gl) override;
    };

}
