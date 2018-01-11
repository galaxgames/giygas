#pragma once
#include "../GLOperation.hpp"
#include <condition_variable>

namespace giygas {
    using namespace std;

    class GLVertexBuffer;

    class DeleteBufferGLOperation : public GLOperation {
        GLuint _handle;
//        mutex _mutex;
//        condition_variable _cv;
    public:
        DeleteBufferGLOperation();
        void set_handle(GLuint handle);
        void execute(GL* gl) override;
        //void wait();
    };

}