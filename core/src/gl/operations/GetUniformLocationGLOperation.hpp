#pragma once
#include "../GLOperation.hpp"
#include <mutex>
#include <condition_variable>

namespace giygas {
    using namespace std;

    class GetUniformLocationGLOperation : public GLOperation {
        GLuint _program;
        const char *_name;
        GLint _location;
        bool _finished;
        mutex _mutex;
        condition_variable _cv;
    public:
        GetUniformLocationGLOperation(GLuint program, const char* name);
        void execute(GL *gl) override;
        GLint get_location();
    };

}