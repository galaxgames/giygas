#pragma once
#include "../GLOperation.hpp"
#include <mutex>
#include <condition_variable>
#include <string>

namespace giygas {
    using namespace std;

    class CompileShaderGLOperation : public GLOperation {
        GLuint _shader;
        string _source;
        bool _success;
        bool _finished;
        string _message;
        mutex _mutex;
        condition_variable _cv;
    public:
        CompileShaderGLOperation(GLuint shader, string &&source);
        void execute(GL *gl) override;
        void wait();
        bool is_successful() const;
        string &message();
    };

}