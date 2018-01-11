#pragma once
#include "../GLOperation.hpp"
#include <mutex>
#include <condition_variable>

namespace giygas {
    using namespace std;

    class GenVertexArrayGLOperation : public GLOperation {
        GLuint _handle;
        bool _finished;
        mutex _mutex;
        condition_variable _cv;
    public:
        GenVertexArrayGLOperation();
        void execute(GL *gl) override;
        GLuint get_generated_handle();
    };
}