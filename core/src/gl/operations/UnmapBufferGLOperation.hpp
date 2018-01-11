#pragma once
#include "../GLOperation.hpp"
#include <mutex>
#include <condition_variable>


namespace giygas {
    using namespace std;

    class UnmapBufferGLOperation : public GLOperation {
        GLuint _handle;
        GLenum _target;
        GLboolean _result;
        bool _finished;
        mutex _mutex;
        condition_variable _cv;
    public:
        UnmapBufferGLOperation(GLuint handle, GLenum target);
        void execute(GL *gl);
        GLboolean get_result();
    };

}