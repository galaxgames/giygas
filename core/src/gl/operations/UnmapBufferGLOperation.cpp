#include "UnmapBufferGLOperation.hpp"

using namespace giygas;

UnmapBufferGLOperation::UnmapBufferGLOperation(GLuint handle, GLenum target) {
    _handle = handle;
    _target = target;
    _finished = false;
}

void UnmapBufferGLOperation::execute(GL *gl) {
    gl->bind_buffer(_target, _handle);
    GLboolean result = gl->unmap_buffer(_target);
    lock_guard<mutex> lock(_mutex);
    _result = result;
    _cv.notify_all();
}

GLboolean UnmapBufferGLOperation::get_result() {
    unique_lock<mutex> lock(_mutex);
    if (_finished) {
        return _result;
    }
    _cv.wait(lock, [this]{ return _finished; });
    return _result;
}