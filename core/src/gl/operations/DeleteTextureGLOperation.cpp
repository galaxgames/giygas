#include "DeleteTextureGLOperation.hpp"

using namespace giygas;
using namespace std;

DeleteTextureGLOperation::DeleteTextureGLOperation() {
    //_finished = false;
}

void DeleteTextureGLOperation::set(GLuint handle) {
    _handle = handle;
}

void DeleteTextureGLOperation::execute(GL *gl) {
    gl->delete_textures(1, &_handle);
//    lock_guard<mutex> lock(_mutex);
//    _finished = true;
//    _cv.notify_all();
}

//void DeleteTextureGLOperation::wait() {
//    unique_lock<mutex> lock(_mutex);
//    if (_finished) {
//        return;
//    }
//    _cv.wait(lock, [this]{ return _finished; });
//}
