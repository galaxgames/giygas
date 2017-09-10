#pragma once

#include <glad.h>

namespace giygas {
    class GL {
    public:
        virtual ~GL() = default;

        //
        // Buffer Functions
        //

        virtual void gen_buffers(GLsizei n, GLuint *buffers) = 0;
        virtual void gen_vertex_arrays(GLsizei n, GLuint *arrays) = 0;

        virtual void delete_buffers(GLsizei n, GLuint *buffers) = 0;
        virtual void delete_vertex_arrays(GLsizei n, GLuint *arrays) = 0;

        virtual void bind_buffer(GLenum target, GLuint handle) = 0;
        virtual void bind_vertex_array(GLuint array) = 0;

        virtual void vertex_attrib_pointer(
            GLuint index, GLint size, GLenum type, GLboolean normalized,
            GLsizei stride, const GLvoid *pointer
        ) = 0;
        virtual void enable_vertex_attrib_array(GLuint index) = 0;

        virtual void buffer_data(
            GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage
        ) = 0;
        virtual void buffer_sub_data(
            GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data
        ) = 0;


        //
        // Program Functions
        //

        virtual GLuint create_program() = 0;
        virtual GLuint create_shader(GLenum type) = 0;

        virtual void delete_program(GLuint program) = 0;
        virtual void delete_shader(GLuint shader) = 0;

        virtual void attach_shader(GLuint program, GLuint shader) = 0;
        virtual void link_program(GLuint program) = 0;
        virtual void get_program_iv(
            GLuint program, GLenum pname, GLint *params
        ) = 0;
        virtual void get_program_info_log(
            GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infolog
        ) = 0;
        virtual void use_program(GLuint program) = 0;
        virtual GLint get_uniform_location(
            GLuint program, const GLchar *name
        ) = 0;

        virtual void uniform_1f(GLint location, GLfloat v0) = 0;
        virtual void uniform_1i(GLint location, GLint v0) = 0;


        //
        // Shader Functions
        //

        virtual void shader_source(
            GLuint shader, GLsizei count, const GLchar *const *string,
            const GLint *length
        ) = 0;
        virtual void compile_shader(GLuint shader) = 0;
        virtual void get_shader_iv(
            GLuint shader, GLenum pname, GLint *params
        ) = 0;
        virtual void get_shader_info_log(
            GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog
        ) = 0;


        //
        // Texture Functions
        //

        virtual void gen_textures(GLsizei n, GLuint *textures) = 0;
        virtual void delete_textures(GLsizei n, GLuint *textures) = 0;
        virtual void bind_texture(GLenum target, GLuint texture) = 0;
        virtual void tex_image_2d(
            GLenum target, GLint level, GLint internalFormat, GLsizei width,
            GLsizei height, GLint border, GLenum format, GLenum type,
            const GLvoid *data
        ) = 0;
        virtual void compressed_tex_image_2d(
            GLenum target, GLint level, GLenum internalformat, GLsizei width,
            GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data
        ) = 0;
        virtual void tex_parameter_i(
            GLenum target, GLenum pname, GLint param
        ) = 0;
    };
}
