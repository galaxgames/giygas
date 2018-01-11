#include "DeleteFramebufferGLOperation.hpp"
#include "../GLVertexBuffer.hpp"

using namespace giygas;
using namespace std;

DeleteFramebufferGLOperation::DeleteFramebufferGLOperation() {
    //_handle = handle;
}

void DeleteFramebufferGLOperation::set_handle(GLuint handle) {
    _handle = handle;
}

void DeleteFramebufferGLOperation::execute(GL *gl) {
    gl->delete_framebuffers(1, &_handle);
//    unique_lock<mutex> lock(_mutex);
//    _handle = 0;
//    _cv.notify_all();
}

//void DeleteBufferGLOperation::wait() {
//    unique_lock<mutex> lock(_mutex);
//    if (_handle == 0) {
//        return;
//    }
//    _cv.wait(lock, [this]{ return _handle == 0; });
//}