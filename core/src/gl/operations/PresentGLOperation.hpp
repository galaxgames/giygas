#pragma once
#include "../GLOperation.hpp"
#include <mutex>
#include <condition_variable>
#include <giygas/GLContext.hpp>

namespace giygas {
    using namespace std;

    class PresentGLOperation : public GLOperation {
        GLContext *_context;
        bool _finished;
        mutex _mutex;
        condition_variable _cv;
    public:
        PresentGLOperation(GLContext *context);
        void execute(GL *gl) override;
        void wait();
    };

}