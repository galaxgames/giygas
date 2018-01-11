#pragma once
#include <mutex>
#include <condition_variable>
#include "../GLOperation.hpp"

namespace giygas {
    using namespace std;

    class CreateProgramGLOperation : public GLOperation {
        GLuint _handle;
        bool _finished;
        mutex _mutex;
        condition_variable _cv;
    public:
        CreateProgramGLOperation();
        void execute(GL *gl) override;
        GLuint get_handle();
    };
}