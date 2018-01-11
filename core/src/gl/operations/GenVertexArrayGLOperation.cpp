#include "GenVertexArrayGLOperation.hpp"

using namespace giygas;

GenVertexArrayGLOperation::GenVertexArrayGLOperation() {
    _finished = false;
}

void GenVertexArrayGLOperation::execute(GL *gl) {
    gl->gen_vertex_arrays(1, &_handle);
    lock_guard<mutex> lock(_mutex);
    _finished = true;
    _cv.notify_all();
}

GLuint GenVertexArrayGLOperation::get_generated_handle() {
    unique_lock<mutex> lock(_mutex);
    if (_finished) {
        return _handle;
    }
    _cv.wait(lock, [this]{ return _finished; });
    return _handle;
}
