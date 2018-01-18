#include "CreateTextureStorageSyncedGLOperation.hpp"

using namespace giygas;

CreateTextureStorageSyncedGLOperation::CreateTextureStorageSyncedGLOperation() {
    _finished = false;
}

void CreateTextureStorageSyncedGLOperation::set(
    GLuint handle,
    GLenum target,
    GLint level,
    GLint internalFormat,
    GLsizei width,
    GLsizei height,
    GLenum format,
    GLenum type,
    const GLvoid *data
) {
    _base.set(
        handle,
        target,
        level,
        internalFormat,
        width,
        height,
        format,
        type,
        data
    );
}

void CreateTextureStorageSyncedGLOperation::execute(GL *gl) {
    _base.execute(gl);
    lock_guard<mutex> lock(_mutex);
    _finished = true;
    _cv.notify_all();
}

void CreateTextureStorageSyncedGLOperation::wait() {
    unique_lock<mutex> lock(_mutex);
    if (_finished) {
        return;
    }
    _cv.wait(lock, [this]{ return _finished; });
}
