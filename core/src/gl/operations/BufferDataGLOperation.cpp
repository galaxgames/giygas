#include "BufferDataGLOperation.hpp"

using namespace giygas;
using namespace std;

BufferDataGLOperation::BufferDataGLOperation(
//    GLuint handle,
//    GLenum target,
//    GLsizei size,
//    GLvoid *data
) {
//    _handle = handle;
//    _target = target;
//    _size = size;
//    _data = data;
    //_finished = false;
}

void BufferDataGLOperation::set(
    GLuint handle,
    GLenum target,
    GLsizei size,
    GLvoid *data
) {
    _handle = handle;
    _target = target;
    _size = size;
    _data = data;
}

void BufferDataGLOperation::execute(GL *gl) {
    gl->bind_buffer(_target, _handle);
    gl->buffer_data(
        _target,
        _size,
        _data,
        GL_DYNAMIC_DRAW
    );
//    lock_guard<mutex> lock(_mutex);
//    _finished = true;
//    _cv.notify_all();
}

//void BufferDataGLOperation::wait() {
//    unique_lock<mutex> lock(_mutex);
//    if (_finished) {
//        return;
//    }
//    _cv.wait(lock, [this]{ return _finished; });
//}