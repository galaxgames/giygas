#pragma once
#include "../GLOperation.hpp"
#include <mutex>
#include <condition_variable>
#include <string>

namespace giygas {
    using namespace std;

    class CompileShaderGLOperation : public GLOperation {
        GLuint _shader;
        unique_ptr<const GLchar[]> _source;
        size_t _length;
//        bool _success;
//        bool _finished;
//        string _message;
//        mutex _mutex;
//        condition_variable _cv;
    public:
        void setup(
            GLuint shader,
            unique_ptr<const GLchar[]> &&source,
            size_t length
        );
        void execute(GL *gl) override;
//        void wait();
//        bool is_successful() const;
//        string &message();
    };

}