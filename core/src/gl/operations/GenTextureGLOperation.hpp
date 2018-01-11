#pragma once
#include "../GLOperation.hpp"
#include <mutex>
#include <condition_variable>

namespace giygas {
    using namespace std;

    class GenTextureGLOperation : public GLOperation {
        GLuint _handle;
        bool _finished;
        mutex _mutex;
        condition_variable _cv;
    public:
        GenTextureGLOperation();
        void execute(GL *gl) override;
        GLuint get_generated_handle();
    };
}
