#include "DeleteRenderbufferGLOperation.hpp"

using namespace giygas;
using namespace std;

DeleteRenderbufferGLOperation::DeleteRenderbufferGLOperation() {
    //_finished = false;
}

void DeleteRenderbufferGLOperation::set(GLuint handle) {
    _handle = handle;
}

void DeleteRenderbufferGLOperation::execute(GL *gl) {
    gl->delete_renderbuffers(1, &_handle);
//    lock_guard<mutex> lock(_mutex);
//    _finished = true;
//    _cv.notify_all();
}

//void DeleteShaderGLOperation::wait() {
//    unique_lock<mutex> lock(_mutex);
//    if (_finished) {
//        return;
//    }
//    _cv.wait(lock, [this]{ return _finished; });
//}
