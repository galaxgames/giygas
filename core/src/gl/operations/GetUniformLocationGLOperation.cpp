#include "GetUniformLocationGLOperation.hpp"

using namespace giygas;
using namespace std;

GetUniformLocationGLOperation::GetUniformLocationGLOperation(
    GLuint program,
    const char *name
) {
    _program = program;
    _name = name;
    _finished = false;
}

void GetUniformLocationGLOperation::execute(GL *gl) {
    _location = gl->get_uniform_location(_program, _name);
    lock_guard<mutex> lock(_mutex);
    _finished = true;
    _cv.notify_all();
}

GLint GetUniformLocationGLOperation::get_location() {
    unique_lock<mutex> lock(_mutex);
    if (_finished) {
        return _location;
    }
    _cv.wait(lock, [this]{ return _finished; });
    return _location;
}
