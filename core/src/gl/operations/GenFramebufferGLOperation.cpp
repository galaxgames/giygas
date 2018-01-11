#include "GenFramebufferGLOperation.hpp"
#include "../GLVertexBuffer.hpp"

using namespace giygas;

GenFramebufferGLOperation::GenFramebufferGLOperation() {
    _finished = false;
}

void GenFramebufferGLOperation::execute(GL *gl) {
    gl->gen_framebuffers(1, &_handle);
    lock_guard<mutex> lock(_mutex);
    _finished = true;
    _cv.notify_all();
}

GLuint GenFramebufferGLOperation::get_generated_handle() {
    unique_lock<mutex> lock(_mutex);
    if (_finished) {
        return _handle;
    }
    _cv.wait(lock, [this]{ return _finished; });
    return _handle;
}