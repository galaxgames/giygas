#pragma once
#include "../../GLOperation.hpp"

namespace giygas {

    class PipelineEnableDisableGLOperation : public GLOperation {
        GLenum _feature;
        bool _value;
    public:
        PipelineEnableDisableGLOperation();
        void set(GLenum feature, bool value);
        void execute(GL *gl) override;
    };

}
