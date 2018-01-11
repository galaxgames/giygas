#include "CreateShaderGLOperation.hpp"

using namespace giygas;

CreateShaderGLOperation::CreateShaderGLOperation(GLenum type) {
    _type = type;
    _finished = false;
}

void CreateShaderGLOperation::execute(GL *gl) {
    _handle = gl->create_shader(_type);
    lock_guard<mutex> lock(_mutex);
    _finished = true;
    _cv.notify_all();
}

GLuint CreateShaderGLOperation::get_handle() {
    unique_lock<mutex> lock(_mutex);
    if (_finished) {
        return _handle;
    }
    _cv.wait(lock, [this]{ return _finished; });
    return _handle;
}
