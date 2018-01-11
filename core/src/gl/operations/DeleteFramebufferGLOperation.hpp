#pragma once
#include "../GLOperation.hpp"
#include <condition_variable>

namespace giygas {
    using namespace std;

    class GLVertexBuffer;

    class DeleteFramebufferGLOperation : public GLOperation {
        GLuint _handle;
//        mutex _mutex;
//        condition_variable _cv;
    public:
        DeleteFramebufferGLOperation();
        void set_handle(GLuint handle);
        void execute(GL* gl) override;
        //void wait();
    };

}