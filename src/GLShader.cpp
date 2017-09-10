#include <cstring>
#include <utility>
#include "giygas_internal/GLShader.hpp"

using namespace giygas;

GLShader::GLShader(GL *gl) {
    _gl = gl;
    _vertex_shader = gl->create_shader(GL_VERTEX_SHADER);
    _fragment_shader = gl->create_shader(GL_FRAGMENT_SHADER);
    _vertex_message = nullptr;
    _fragment_message = nullptr;
}

GLShader::GLShader(GLShader &&other) noexcept {
    *this = std::move(other);
}

GLShader& GLShader::operator=(GLShader &&other) noexcept {
    _gl = other._gl;
    _vertex_message = other._vertex_message;
    _fragment_message = other._fragment_message;
    _vertex_shader = other._vertex_shader;
    _fragment_shader = other._fragment_shader;
    other._vertex_shader = 0;
    other._fragment_shader = 0;
    other._vertex_message = nullptr;
    other._fragment_message = nullptr;

    return *this;
}

GLShader::~GLShader() {
    delete[] _vertex_message;
    delete[] _fragment_message;
    _gl->delete_shader(_vertex_shader);
    _gl->delete_shader(_fragment_shader);
}

RendererType GLShader::renderer_type() const {
    return RendererType::OpenGL;
}

void GLShader::set_from_source(const char* vertex, const char* fragment) {
    bool failed = false;
    failed |= !compile_shader(_vertex_shader, vertex, &_vertex_message);
    failed |= !compile_shader(_fragment_shader, fragment, &_fragment_message);
    _is_valid = !failed;
}

bool GLShader::compile_shader(
    GLuint shader, const char *source, char **message
) {
    delete[] *message;

    _gl->shader_source(
        shader,
        1,  // only passing 1 string,
        &source,
        nullptr // null pointer to string lengths because source strings are
                // null terminated.
    );
    _gl->compile_shader(shader);
    GLint status;
    _gl->get_shader_iv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_TRUE) {
        const char* no_error_message = "No error";
        // TODO: Get length of no error message at compile time
        char *log = new char[strlen(no_error_message) + 1];
        strcpy(log, no_error_message);
        *message = log;
    }
    else {
        GLint length;
        GLsizei retrieved;
        _gl->get_shader_iv(shader, GL_INFO_LOG_LENGTH, &length);
        char *log = new char[length];
        GLint offset = 0;
        while (offset < length - 1) {
            _gl->get_shader_info_log(
                shader, length - offset, &retrieved, log + offset
            );
            offset += retrieved;
        }
        log[length - 1] = '\0';
        *message = log;
    }

    return status == GL_TRUE;
}

bool GLShader::is_valid() const {
    return _is_valid;
}

const char *GLShader::get_vertex_message() const {
    return _vertex_message;
}

const char* GLShader::get_fragment_message() const {
    return _fragment_message;
}

GLuint GLShader::get_vertex_shader() const {
    return _vertex_shader;
}

GLuint GLShader::get_fragment_shader() const {
    return _fragment_shader;
}
