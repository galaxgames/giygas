#include "DeleteProgramGLOperation.hpp"

using namespace giygas;
using namespace std;

DeleteProgramGLOperation::DeleteProgramGLOperation() {
    //_finished = false;
}

void DeleteProgramGLOperation::set(GLuint handle) {
    _handle = handle;
}

void DeleteProgramGLOperation::execute(GL *gl) {
    gl->delete_program(_handle);
//    lock_guard<mutex> lock(_mutex);
//    _finished = true;
//    _cv.notify_all();
}

//void DeleteProgramGLOperation::wait() {
//    unique_lock<mutex> lock(_mutex);
//    if (_finished) {
//        return;
//    }
//    _cv.wait(lock, [this]{ return _finished; });
//}
