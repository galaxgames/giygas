#pragma once
#include <mutex>
#include <condition_variable>
#include "../GLOperation.hpp"

namespace giygas {
    using namespace std;

    class DeleteShaderGLOperation : public GLOperation {
        GLuint _handle;
//        bool _finished;
//        mutex _mutex;
//        condition_variable _cv;
    public:
        DeleteShaderGLOperation();
        void set(GLuint handle);
        void execute(GL *gl) override;
        //void wait();
    };
}
