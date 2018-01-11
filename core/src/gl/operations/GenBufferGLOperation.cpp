#include "GenBufferGLOperation.hpp"
#include "../GLVertexBuffer.hpp"

using namespace giygas;

GenBufferGLOperation::GenBufferGLOperation() {
    _finished = false;
}

void GenBufferGLOperation::execute(GL *gl) {
    gl->gen_buffers(1, &_handle);
    lock_guard<mutex> lock(_mutex);
    _finished = true;
    _cv.notify_all();
}

GLuint GenBufferGLOperation::get_generated_handle() {
    unique_lock<mutex> lock(_mutex);
    if (_finished) {
        return _handle;
    }
    _cv.wait(lock, [this]{ return _finished; });
    return _handle;
}