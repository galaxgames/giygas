#pragma once
#include "../GLOperation.hpp"
#include <mutex>
#include <condition_variable>

namespace giygas {
    using namespace std;

    class SetTextureParameterGLOperation : public GLOperation {
        GLuint _handle;
        GLenum _target;
        GLenum _parameter;
        GLint _value;
//        bool _finished;
//        mutex _mutex;
//        condition_variable _cv;
    public:
        SetTextureParameterGLOperation();
        void execute(GL *gl) override;
        void set(
            GLuint handle,
            GLenum target,
            GLenum parameter,
            GLint value
        );
    };
}
