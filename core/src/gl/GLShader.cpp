#include <cstring>
#include <utility>
#include "GLShader.hpp"
#include "GLRenderer.hpp"
#include "operations/CreateShaderGLOperation.hpp"
#include "operations/DeleteShaderGLOperation.hpp"
#include "operations/CompileShaderGLOperation.hpp"

using namespace giygas;

GLShader::GLShader(GLRenderer *renderer) {
    _renderer = renderer;

    CreateShaderGLOperation create_vs_op(GL_VERTEX_SHADER);
    CreateShaderGLOperation create_fs_op(GL_FRAGMENT_SHADER);

    _renderer->add_operation(&create_vs_op, nullptr);
    _renderer->add_operation_and_notify(&create_fs_op, nullptr);

    _vertex_shader = create_vs_op.get_handle();
    _fragment_shader = create_fs_op.get_handle();
}

GLShader::GLShader(GLShader &&other) noexcept {
    *this = std::move(other);
}

GLShader& GLShader::operator=(GLShader &&other) noexcept {
    _renderer = other._renderer;
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
    Pool<DeleteShaderGLOperation> &pool = _renderer->pools().delete_shader_ops;
    DeleteShaderGLOperation *del_vs_op = pool.take();
    DeleteShaderGLOperation *del_fs_op = pool.take();
    del_vs_op->set(_vertex_shader);
    del_fs_op->set(_fragment_shader);
    _renderer->add_operation(del_vs_op, &pool);
    _renderer->add_operation(del_fs_op, &pool);
}

RendererType GLShader::renderer_type() const {
    return RendererType::OpenGL;
}

void GLShader::set_from_source(const char* vertex, const char* fragment) {
    bool failed = false;
    failed |= !compile_shader(_vertex_shader, vertex, _vertex_message);
    failed |= !compile_shader(_fragment_shader, fragment, _fragment_message);
    _is_valid = !failed;
}

bool GLShader::compile_shader(
    GLuint shader, const char *source, string &message
) {
    CompileShaderGLOperation compile_op(shader, source);
    _renderer->add_operation_and_notify(&compile_op, nullptr);
    compile_op.wait();
    message = move(compile_op.message());
    return compile_op.is_successful();
}

bool GLShader::is_valid() const {
    return _is_valid;
}

const string &GLShader::get_vertex_message() const {
    return _vertex_message;
}

const string &GLShader::get_fragment_message() const {
    return _fragment_message;
}

GLuint GLShader::get_vertex_shader() const {
    return _vertex_shader;
}

GLuint GLShader::get_fragment_shader() const {
    return _fragment_shader;
}
