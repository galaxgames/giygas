#include "BufferSubDataGLOperation.hpp"

using namespace giygas;

BufferSubDataGLOperation::BufferSubDataGLOperation(
) {
    //_finished = false;
}

void BufferSubDataGLOperation::set(
    GLuint handle,
    GLenum target,
    GLsizei offset,
    GLsizei count,
    const GLvoid *data
) {
    _handle = handle;
    _target = target;
    _offset = offset;
    _count = count;
    _data = data;
}

void BufferSubDataGLOperation::execute(GL *gl) {
    gl->bind_buffer(_target, _handle);
    gl->buffer_sub_data(
        _target,
        _offset,
        _count,
        _data
    );
//    lock_guard<mutex> lock(_mutex);
//    _finished = true;
//    _cv.notify_all();
}

//void BufferSubDataGLOperation::wait() {
//    unique_lock<mutex> lock(_mutex);
//    if (_finished) {
//        return;
//    }
//    _cv.wait(lock, [this]{ return _finished; });
//}