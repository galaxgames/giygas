#include "PresentGLOperation.hpp"

using namespace giygas;

PresentGLOperation::PresentGLOperation(GLContext *context) {
    _context = context;
    _finished = false;
}

void PresentGLOperation::execute(GL *gl) {
    _context->present();
    lock_guard<mutex> lock(_mutex);
    _finished = true;
    _cv.notify_all();
}

void PresentGLOperation::wait() {
    unique_lock<mutex> lock(_mutex);
    if (_finished) {
        return;
    }
    _cv.wait(lock, [this]{ return _finished; });
}
