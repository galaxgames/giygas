#include "CompileShaderGLOperation.hpp"

using namespace giygas;

void CompileShaderGLOperation::setup(
    GLuint shader,
    unique_ptr<const GLchar[]> &&source,
    size_t length
) {
    _shader = shader;
    _source = move(source);
    _length = length;
    //_finished = false;
}

void CompileShaderGLOperation::execute(GL *gl) {
    const GLchar *source_ptr = _source.get();
    gl->shader_source(
        _shader,
        1,  // only passing 1 string,
        &source_ptr,
        nullptr  // null pointer to string lengths because source strings are
                 // null terminated.
    );
    gl->compile_shader(_shader);
//    GLint status;
//    gl->get_shader_iv(_shader, GL_COMPILE_STATUS, &status);
//    _success = status == GL_TRUE;
//
//    GLint length;
//    GLsizei retrieved;
//    gl->get_shader_iv(_shader, GL_INFO_LOG_LENGTH, &length);
//    _message.clear();
//    if (length > 0) {
//        _message.resize(static_cast<string::size_type>(length - 1));
//        //_message.reserve(static_cast<string::size_type>(length - 1));
//    }
//    GLint offset = 0;
//    while (offset < length - 1) {
//        gl->get_shader_info_log(
//            _shader, length - offset, &retrieved, &_message[offset]
//        );
//        offset += retrieved;
//    }
//
//    lock_guard<mutex> lock(_mutex);
//    _finished = true;
//    _cv.notify_all();
}

//void CompileShaderGLOperation::wait() {
//    unique_lock<mutex> lock(_mutex);
//    if (_finished) {
//        return;
//    }
//    _cv.wait(lock, [this]{ return _finished; });
//}
//
//bool CompileShaderGLOperation::is_successful() const {
//    return _success;
//}
//
//string &CompileShaderGLOperation::message() {
//    return _message;
//}
