#include "DeleteBufferGLOperation.hpp"
#include "../GLVertexBuffer.hpp"

using namespace giygas;
using namespace std;

DeleteBufferGLOperation::DeleteBufferGLOperation() {
    //_handle = handle;
}

void DeleteBufferGLOperation::set_handle(GLuint handle) {
    _handle = handle;
}

void DeleteBufferGLOperation::execute(GL *gl) {
    gl->delete_buffers(1, &_handle);
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