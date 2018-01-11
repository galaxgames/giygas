#pragma once
#include <mutex>
#include <condition_variable>
#include "../GLOperation.hpp"

namespace giygas {
    using namespace std;

    class CreateShaderGLOperation : public GLOperation {
        GLenum _type;
        GLuint _handle;
        bool _finished;
        mutex _mutex;
        condition_variable _cv;
    public:
        CreateShaderGLOperation(GLenum type);
        void execute(GL *gl) override;
        GLuint get_handle();
    };
}