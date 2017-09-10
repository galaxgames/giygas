#pragma once

#include <gmock/gmock.h>
#include <giygas/GL.hpp>

namespace giygas {
    class MockGL : public GL {
    public:
        MOCK_METHOD2(gen_buffers, void(GLsizei n, GLuint *buffers));
        MOCK_METHOD2(gen_vertex_arrays, void(GLsizei n, GLuint *arrays));

        MOCK_METHOD2(delete_buffers, void(GLsizei n, GLuint *buffers));
        MOCK_METHOD2(delete_vertex_arrays, void(GLsizei n, GLuint *arrays));

        MOCK_METHOD2(bind_buffer, void(GLenum target, GLuint handle));
        MOCK_METHOD1(bind_vertex_array, void(GLuint array));

        MOCK_METHOD6(vertex_attrib_pointer, void(
            GLuint index, GLint size, GLenum type, GLboolean normalized,
            GLsizei stride, const GLvoid *pointer
        ));
        MOCK_METHOD1(enable_vertex_attrib_array, void(GLuint index));

        MOCK_METHOD4(buffer_data, void(
            GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage
        ));
        MOCK_METHOD4(buffer_sub_data, void(
            GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data
        ));

        MOCK_METHOD0(create_program, GLuint());
        MOCK_METHOD1(create_shader, GLuint(GLenum type));

        MOCK_METHOD1(delete_program, void(GLuint program));
        MOCK_METHOD1(delete_shader, void(GLuint shader));

        MOCK_METHOD2(attach_shader, void(GLuint program, GLuint shader));
        MOCK_METHOD1(link_program, void(GLuint program));
        MOCK_METHOD3(get_program_iv, void(
            GLuint program, GLenum pname, GLint *params
        ));
        MOCK_METHOD4(get_program_info_log, void(
           GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infolog
        ));
        MOCK_METHOD1(use_program, void(GLuint program));
        MOCK_METHOD2(get_uniform_location, GLint(
            GLuint program, const GLchar *name
        ));
        MOCK_METHOD2(uniform_1f, void(GLint location, GLfloat v0));
        MOCK_METHOD2(uniform_1i, void(GLint location, GLint v0));

        MOCK_METHOD4(shader_source, void(
            GLuint shader, GLsizei count, const GLchar *const *string,
            const GLint *length
        ));
        MOCK_METHOD1(compile_shader, void(GLuint shader));
        MOCK_METHOD3(get_shader_iv, void(
            GLuint shader, GLenum pname, GLint *params
        ));
        MOCK_METHOD4(get_shader_info_log, void(
            GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog
        ));

        MOCK_METHOD2(gen_textures, void(GLsizei n, GLuint *textures));
        MOCK_METHOD2(delete_textures, void(GLsizei n, GLuint *textures));
        MOCK_METHOD2(bind_texture, void(GLenum target, GLuint texture));
        MOCK_METHOD8(compressed_tex_image_2d, void(
            GLenum target, GLint level, GLenum internalformat, GLsizei width,
            GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data
        ));
        MOCK_METHOD9(tex_image_2d, void(
            GLenum target, GLint level, GLint internalFormat, GLsizei width,
            GLsizei height, GLint border, GLenum format, GLenum type,
            const GLvoid *data
        ));
        MOCK_METHOD3(tex_parameter_i, void(
            GLenum target, GLenum pname, GLint param
        ));

    };
}
