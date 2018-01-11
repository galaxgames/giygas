#pragma once
#include "../GLOperation.hpp"
#include "../GLUniformValue.hpp"
#include <mutex>
#include <condition_variable>


namespace giygas {
    using namespace std;

    class SetUniformValueGLOperation : public GLOperation {
        GLuint _program;
        GLint _location;
        GLUniformValue *_uniform_value;
//        bool _finished;
//        mutex _mutex;
//        condition_variable _cv;
    public:
        SetUniformValueGLOperation();
        void set(
            GLuint program,
            GLint location,
            GLUniformValue *uniform_value
        );
        void execute(GL *gl) override;
        //void wait();
    };
}