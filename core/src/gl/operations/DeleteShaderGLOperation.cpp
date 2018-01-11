#include "DeleteShaderGLOperation.hpp"

using namespace giygas;
using namespace std;

DeleteShaderGLOperation::DeleteShaderGLOperation() {
    //_finished = false;
}

void DeleteShaderGLOperation::set(GLuint handle) {
    _handle = handle;
}

void DeleteShaderGLOperation::execute(GL *gl) {
    gl->delete_shader(_handle);
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
