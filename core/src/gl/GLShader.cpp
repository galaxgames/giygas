#include <cstring>
#include <utility>
#include <algorithm>
#include "GLShader.hpp"
#include "GLRenderer.hpp"
#include "operations/CreateShaderGLOperation.hpp"
#include "operations/DeleteShaderGLOperation.hpp"
#include "operations/CompileShaderGLOperation.hpp"

using namespace giygas;

GLShader::GLShader(GLRenderer *renderer) {
    _renderer = renderer;
    _handle = 0;
}

GLShader::GLShader(GLShader &&other) noexcept {
    *this = std::move(other);
}

GLShader& GLShader::operator=(GLShader &&other) noexcept {
    _renderer = other._renderer;
    _handle = other._handle;
    _type = other._type;
    other._handle = 0;
    return *this;
}

GLShader::~GLShader() {
    delete_shader();
}

RendererType GLShader::renderer_type() const {
    return RendererType::OpenGL;
}


void GLShader::set_code(
    const uint8_t *code,
    size_t length,
    ShaderType type
) {
    // Hooray! OpenGL is wonderful!!!
    GLchar *sanitized_source = new GLchar[length];
    for (size_t i = 0; i < length; ++i) {
        sanitized_source[i] = static_cast<GLchar>(code[i]);
    }

    // Create or re-create the shader if needed.
    if (_handle == 0 || type != _type) {
        _type = type;
        delete_shader();
        create_shader(type);
    }

    // Start the operation
    Pool<CompileShaderGLOperation> &pool =
        _renderer->pools().compile_shader_ops;
    CompileShaderGLOperation *compile_op = pool.take();
    compile_op->setup(
        _handle,
        unique_ptr<const GLchar[]>(sanitized_source),
        length
    );
    _renderer->add_operation(compile_op, nullptr);
}

void GLShader::create_shader(ShaderType type) {
    CreateShaderGLOperation create_op(shader_type_to_enum(type));
    _renderer->add_operation_and_notify(&create_op, nullptr);
    _handle = create_op.get_handle();
}

void GLShader::delete_shader() {
    Pool<DeleteShaderGLOperation> &pool = _renderer->pools().delete_shader_ops;
    DeleteShaderGLOperation *del_op = pool.take();
    del_op->set(_handle);
    _renderer->add_operation(del_op, &pool);
    _handle = 0;
}

GLenum GLShader::shader_type_to_enum(ShaderType type) {
    switch(type) {
        case ShaderType::Vertex:
            return GL_VERTEX_SHADER;
        case ShaderType::Fragment:
            return GL_FRAGMENT_SHADER;
    }
}
