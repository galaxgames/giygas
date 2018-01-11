#pragma once
#include "../GLOperation.hpp"
#include <mutex>
#include <condition_variable>

namespace giygas {
    using namespace std;

    class BufferSubDataGLOperation : public GLOperation {
        GLuint _handle;
        GLenum _target;
        GLsizei _offset;
        GLsizei _count;
        const GLvoid *_data;
//        mutex _mutex;
//        condition_variable _cv;
//        bool _finished;

    public:
        BufferSubDataGLOperation();
        void set(
            GLuint handle,
            GLenum target,
            GLsizei offset,
            GLsizei count,
            const GLvoid *data
        );
        void execute(GL *gl) override;
        //void wait();
    };


}