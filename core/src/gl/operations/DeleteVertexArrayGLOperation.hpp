#pragma once
#include "../GLOperation.hpp"
#include <mutex>
#include <condition_variable>

namespace giygas {
    using namespace std;

    class DeleteVertexArrayGLOperation : public GLOperation {
        GLuint _handle;
//        mutex _mutex;
//        condition_variable _cv;
    public:
        DeleteVertexArrayGLOperation();
        void set(GLuint handle);
        void execute(GL *gl) override;
        //void wait();
    };

}