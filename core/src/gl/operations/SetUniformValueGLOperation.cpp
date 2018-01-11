#include "SetUniformValueGLOperation.hpp"

using namespace giygas;
using namespace std;

SetUniformValueGLOperation::SetUniformValueGLOperation(
) {
    //_finished = false;
}

void SetUniformValueGLOperation::set(
    GLuint program,
    GLint location,
    GLUniformValue *uniform_value
) {
    _program = program;
    _location = location;
    _uniform_value = uniform_value;
}

void SetUniformValueGLOperation::execute(GL *gl) {
    gl->use_program(_program);
    _uniform_value->do_gl_call(gl, _location);
//    lock_guard<mutex> lock(_mutex);
//    _finished = false;
//    _cv.notify_all();
}

//void SetUniformValueGLOperation::wait() {
//    unique_lock<mutex> lock(_mutex);
//    if (_finished) {
//        return;
//    }
//    _cv.wait(lock, [this]{ return _finished; });
//}
