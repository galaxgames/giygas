#pragma once

#include <giygas/GL.hpp>

namespace giygas {
    class GLImpl : public GL {
    public:

        GLImpl();
        GLImpl(const GLImpl &) = delete;
        GLImpl(GLImpl &&) noexcept;
        GLImpl &operator=(const GLImpl &) = delete;
        GLImpl &operator=(GLImpl &&) noexcept;
        virtual ~GLImpl();

        void gen_buffers(GLsizei n, GLuint *buffers) override ;
        void gen_vertex_arrays(GLsizei n, GLuint *arrays) override;

        void delete_buffers(GLsizei n, GLuint *buffers) override;
        void delete_vertex_arrays(GLsizei n, GLuint *arrays) override;

        void bind_buffer(GLenum target, GLuint handle) override;
        void bind_vertex_array(GLuint array) override;

        void vertex_attrib_pointer(
            GLuint index, GLint size, GLenum type, GLboolean normalized,
            GLsizei stride, const GLvoid *pointer
        ) override;
        void enable_vertex_attrib_array(GLuint index) override;

        void buffer_data(
            GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage
        ) override;
        void buffer_sub_data(
            GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data
        ) override;

        GLuint create_program() override;
        GLuint create_shader(GLenum type) override;

        void delete_program(GLuint program) override;
        void delete_shader(GLuint shader) override;

        void attach_shader(GLuint program, GLuint shader) override;
        void link_program(GLuint program) override ;
        void get_program_iv(
            GLuint program, GLenum pname, GLint *params
        ) override;
        void get_program_info_log(
            GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infolog
        ) override;
        void use_program(GLuint program) override;
        GLint get_uniform_location(
            GLuint program, const GLchar *name
        ) override;

        void uniform_1f(GLint location, GLfloat v0) override;
        void uniform_1i(GLint location, GLint v0) override;

        void shader_source(
            GLuint shader, GLsizei count, const GLchar *const *string,
            const GLint *length
        ) override;
        void compile_shader(GLuint shader) override;
        void get_shader_iv(
            GLuint shader, GLenum pname, GLint *params
        ) override;
        void get_shader_info_log(
            GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog
        ) override;
    };
}