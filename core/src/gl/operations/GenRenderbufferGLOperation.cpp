#include "GenRenderbufferGLOperation.hpp"

using namespace giygas;

GenRenderbufferGLOperation::GenRenderbufferGLOperation() {
    _finished = false;
}

void GenRenderbufferGLOperation::execute(GL *gl) {
    gl->gen_renderbuffers(1, &_handle);
    lock_guard<mutex> lock(_mutex);
    _finished = true;
    _cv.notify_all();
}

GLuint GenRenderbufferGLOperation::get_generated_handle() {
    unique_lock<mutex> lock(_mutex);
    if (_finished) {
        return _handle;
    }
    _cv.wait(lock, [this]{ return _finished; });
    return _handle;
}
