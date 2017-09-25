#include "giygas/GLImpl.hpp"

using namespace giygas;

GLImpl::GLImpl() {

}

GLImpl::GLImpl(GLImpl &&) noexcept = default;
GLImpl& GLImpl::operator=(GLImpl &&) noexcept = default;

GLImpl::~GLImpl() {

}

void GLImpl::gen_buffers(GLsizei n, GLuint *buffers) {
    glGenBuffers(n, buffers);
}

void GLImpl::gen_vertex_arrays(GLsizei n, GLuint *arrays) {
    glGenVertexArrays(n, arrays);
}

void GLImpl::delete_buffers(GLsizei n, GLuint *buffers) {
    glDeleteBuffers(n, buffers);
}

void GLImpl::delete_vertex_arrays(GLsizei n, GLuint *arrays) {
    glDeleteVertexArrays(n, arrays);
}

void GLImpl::bind_buffer(GLenum target, GLuint handle) {
    glBindBuffer(target, handle);
}

void GLImpl::bind_vertex_array(GLuint array) {
    glBindVertexArray(array);
}

void GLImpl::vertex_attrib_pointer(
    GLuint index, GLint size, GLenum type, GLboolean normalized,
    GLsizei stride, const GLvoid *pointer
) {
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

void GLImpl::enable_vertex_attrib_array(GLuint index) {
    glEnableVertexAttribArray(index);
}

void GLImpl::buffer_data(
    GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage
) {
    glBufferData(target, size, data, usage);
}

void GLImpl::buffer_sub_data(
    GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data
) {
    glBufferSubData(target, offset, size, data);
}

GLuint GLImpl::create_program() {
    return glCreateProgram();
}

GLuint GLImpl::create_shader(GLenum type) {
    return glCreateShader(type);
}

void GLImpl::delete_program(GLuint program) {
    glDeleteProgram(program);
}

void GLImpl::delete_shader(GLuint shader) {
    glDeleteShader(shader);
}

void GLImpl::attach_shader(GLuint program, GLuint shader) {
    glAttachShader(program, shader);
}

void GLImpl::link_program(GLuint program) {
    glLinkProgram(program);
}

void GLImpl::get_program_iv(GLuint program, GLenum pname, GLint *params) {
    glGetProgramiv(program, pname, params);
}

void GLImpl::get_program_info_log(
    GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infolog
) {
    glGetProgramInfoLog(program, bufSize, length, infolog);
}

void GLImpl::use_program(GLuint program) {
    glUseProgram(program);
}

GLint GLImpl::get_uniform_location(GLuint program, const GLchar *name) {
    return glGetUniformLocation(program, name);
}

void GLImpl::uniform_1f(GLint location, GLfloat v0) {
    glUniform1f(location, v0);
}

void GLImpl::uniform_1i(GLint location, GLint v0) {
    glUniform1i(location, v0);
}

void GLImpl::uniform_2f(GLint location, GLfloat v0, GLfloat v1) {
    glUniform2f(location, v0, v1);
}

void GLImpl::uniform_3f(
    GLint location, GLfloat v0, GLfloat v1, GLfloat v2
) {
    glUniform3f(location, v0, v1, v2);
}

void GLImpl::uniform_4f(
    GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3
) {
    glUniform4f(location, v0, v1, v2, v3);
}

void GLImpl::uniform_matrix_4fv(
    GLint location, GLsizei count, GLboolean transpose, const GLfloat *value
) {
    glUniformMatrix4fv(location, count, transpose, value);
}

void GLImpl::shader_source(
    GLuint shader, GLsizei count, const GLchar *const *string,
    const GLint *length
) {
    glShaderSource(shader, count, string, length);
}

void GLImpl::compile_shader(GLuint shader) {
    glCompileShader(shader);
}

void GLImpl::get_shader_iv(GLuint shader, GLenum pname, GLint *params) {
    glGetShaderiv(shader, pname, params);
}

void GLImpl::get_shader_info_log(
    GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog
) {
    glGetShaderInfoLog(shader, bufSize, length, infoLog);
}

void GLImpl::gen_textures(GLsizei n, GLuint *textures) {
    glGenTextures(n, textures);
}

void GLImpl::delete_textures(GLsizei n, GLuint *textures) {
    glDeleteTextures(n, textures);
}

void GLImpl::bind_texture(GLenum target, GLuint texture) {
    glBindTexture(target, texture);
}

void GLImpl::tex_image_2d(
    GLenum target, GLint level, GLint internalFormat, GLsizei width,
    GLsizei height, GLint border, GLenum format, GLenum type,
    const GLvoid *data
) {
    glTexImage2D(
        target, level, internalFormat, width, height, border, format, type,
        data
    );
}

void GLImpl::compressed_tex_image_2d(
    GLenum target, GLint level, GLenum internalformat, GLsizei width,
    GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data
) {
    glCompressedTexImage2D(
        target, level, internalformat, width, height, border, imageSize, data
    );
}

void GLImpl::tex_parameter_i(GLenum target, GLenum pname, GLint param) {
    glTexParameteri(target, pname, param);
}
