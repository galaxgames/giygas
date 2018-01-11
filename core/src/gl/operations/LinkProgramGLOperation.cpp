#include "LinkProgramGLOperation.hpp"

using namespace giygas;
using namespace std;

LinkProgramGLOperation::LinkProgramGLOperation(
    GLuint program,
    GLuint vertex_shader,
    GLuint fragment_shader
) {
    _program = program;
    _vertex_shader = vertex_shader;
    _fragment_shader = fragment_shader;
    _finished = false;
}

void LinkProgramGLOperation::execute(GL *gl) {
    gl->attach_shader(_program, _vertex_shader);
    gl->attach_shader(_program, _fragment_shader);
    gl->link_program(_program);

    GLint status;
    gl->get_program_iv(_program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint log_length;
        GLint log_offset = 0;
        GLsizei retrieved;
        gl->get_program_iv(_program, GL_INFO_LOG_LENGTH, &log_length);
        _message.clear();
        if (log_length > 0) {
            _message.resize(static_cast<string::size_type>(log_length - 1));
            //_message.reserve();
        }
        while (log_offset < log_length - 1) {
            gl->get_program_info_log(
                _program,
                log_length - log_offset,
                &retrieved,
                &_message[log_offset]
            );
            log_offset += retrieved;
        }
        //_message[log_length - 1] = '\0';
        _success = false;
    }
    else {
        _success = true;
    }

    lock_guard<mutex> lock(_mutex);
    _finished = true;
    _cv.notify_all();
}

void LinkProgramGLOperation::wait() {
    unique_lock<mutex> lock(_mutex);
    if (_finished) {
        return;
    }
    _cv.wait(lock, [this]{ return _finished; });
}

bool LinkProgramGLOperation::is_success() const {
    return _success;
}

string& LinkProgramGLOperation::message() {
    return _message;
}
