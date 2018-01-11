#pragma once
#include "../GLOperation.hpp"
#include <string>
#include <mutex>
#include <condition_variable>

namespace giygas {
    using namespace std;

    class LinkProgramGLOperation : public GLOperation {
        GLuint _program;
        GLuint _vertex_shader;
        GLuint _fragment_shader;
        string _message;
        bool _success;
        bool _finished;
        mutex _mutex;
        condition_variable _cv;
    public:
        LinkProgramGLOperation(
            GLuint program,
            GLuint vertex_shader,
            GLuint fragment_shader
        );
        void execute(GL *gl) override;
        void wait();
        bool is_success() const;
        string &message();
    };
}