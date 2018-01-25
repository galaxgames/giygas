#pragma once
#include "../GLOperation.hpp"
#include <mutex>
#include <condition_variable>

namespace giygas {
    using namespace std;

    class GenRenderbufferGLOperation : public GLOperation {
        GLuint _handle;
        bool _finished;
        mutex _mutex;
        condition_variable _cv;
    public:
        GenRenderbufferGLOperation();
        void execute(GL *gl) override;
        GLuint get_generated_handle();
    };
}