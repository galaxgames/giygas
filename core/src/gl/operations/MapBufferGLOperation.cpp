#include "MapBufferGLOperation.hpp"

using namespace giygas;

MapBufferGLOperation::MapBufferGLOperation(
    GLuint handle,
    GLenum target,
    GLenum access
) {
    _handle = handle;
    _target = target;
    _access = access;
    _finished = false;
}

void MapBufferGLOperation::execute(GL *gl) {
    gl->bind_buffer(_target, _handle);
    void *mapped = gl->map_buffer(_target, _access);
    lock_guard<mutex> lock(_mutex);
    _mapped = mapped;
    _cv.notify_all();
}

void* MapBufferGLOperation::get_mapped() {
    unique_lock<mutex> lock(_mutex);
    if (_finished) {
        return _mapped;
    }
    _cv.wait(lock, [this]{ return _finished; });
    return _mapped;
}