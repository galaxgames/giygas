#include "CreateProgramGLOperation.hpp"

using namespace giygas;

CreateProgramGLOperation::CreateProgramGLOperation() {
    _finished = false;
}

void CreateProgramGLOperation::execute(GL *gl) {
    _handle = gl->create_program();
    lock_guard<mutex> lock(_mutex);
    _finished = true;
    _cv.notify_all();
}

GLuint CreateProgramGLOperation::get_handle() {
    unique_lock<mutex> lock(_mutex);
    if (_finished) {
        return _handle;
    }
    _cv.wait(lock, [this]{ return _finished; });
    return _handle;
}