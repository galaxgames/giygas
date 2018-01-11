#pragma once
#include "../GLOperation.hpp"
#include <mutex>
#include <condition_variable>

namespace giygas {
    using namespace std;

    class BufferDataGLOperation : public GLOperation {
        GLuint _handle;
        GLenum _target;
        GLsizei _size;
        GLvoid *_data;
//        mutex _mutex;
//        condition_variable _cv;
//        bool _finished;

    public:
        BufferDataGLOperation();
        void set(GLuint handle, GLenum target, GLsizei size, GLvoid *data);
        void execute(GL *gl) override;
        //void wait();
    };

}