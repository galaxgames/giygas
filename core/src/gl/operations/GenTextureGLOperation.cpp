#include "GenTextureGLOperation.hpp"
#include "../GLVertexBuffer.hpp"

using namespace giygas;

GenTextureGLOperation::GenTextureGLOperation() {
    _finished = false;
}

void GenTextureGLOperation::execute(GL *gl) {
    gl->gen_textures(1, &_handle);
    lock_guard<mutex> lock(_mutex);
    _finished = true;
    _cv.notify_all();
}

GLuint GenTextureGLOperation::get_generated_handle() {
    unique_lock<mutex> lock(_mutex);
    if (_finished) {
        return _handle;
    }
    _cv.wait(lock, [this]{ return _finished; });
    return _handle;
}
