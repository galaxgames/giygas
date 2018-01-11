#pragma once

#include "GL.hpp"

namespace giygas {
    class GLImpl : public GL {
    public:

        GLImpl();
        GLImpl(const GLImpl &) = delete;
        GLImpl(GLImpl &&) noexcept;
        GLImpl &operator=(const GLImpl &) = delete;
        GLImpl &operator=(GLImpl &&) noexcept;
        virtual ~GLImpl();

        //
        // Clear Functions
        //

        void clear_color(
            GLfloat red,
            GLfloat green,
            GLfloat blue,
            GLfloat alpha
        ) override;
        void clear_depth(GLdouble depth) override;
        void clear_stencil(GLint s) override;
        void clear(GLbitfield mask) override;


        //
        // Buffer Functions
        //

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

        void *map_buffer(GLenum target, GLenum access) override;
        GLboolean unmap_buffer(GLenum target) override;


        //
        // Program Functions
        //

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
        void uniform_2f(GLint location, GLfloat v0, GLfloat v1) override;
        void uniform_3f(
            GLint location, GLfloat v0, GLfloat v1, GLfloat v2
        ) override;
        void uniform_4f(
            GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3
        ) override;
        void uniform_matrix_4fv(
            GLint location, GLsizei count, GLboolean transpose,
            const GLfloat *value
        ) override;


        //
        // Shader Functions
        //

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


        //
        // Texture Functions
        //

        void gen_textures(GLsizei n, GLuint *textures) override;
        void delete_textures(GLsizei n, GLuint *textures) override;
        void bind_texture(GLenum target, GLuint texture) override;
        void tex_image_2d(
            GLenum target, GLint level, GLint internalFormat, GLsizei width,
            GLsizei height, GLint border, GLenum format, GLenum type,
            const GLvoid *data
        ) override;
        void compressed_tex_image_2d(
            GLenum target, GLint level, GLenum internalformat, GLsizei width,
            GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data
        ) override;
        void tex_parameter_i(
            GLenum target, GLenum pname, GLint param
        ) override;
        void active_texture(GLenum texture_unit) override;

        //
        // Frame Buffer Functions
        //

        void gen_framebuffers(GLsizei n, GLuint *ids) override;
        void delete_framebuffers(GLsizei n, GLuint *ids) override;
        void bind_framebuffer(GLenum target, GLuint framebuffer) override;
        void framebuffer_texture_2d(
            GLenum target,
            GLenum attachment,
            GLenum textarget,
            GLuint texture,
            GLuint level
        ) override;
        void framebuffer_renderbuffer(
            GLenum target,
            GLenum attachment,
            GLenum renderbuffertarget,
            GLuint renderbuffer
        ) override;


        //
        // Render Buffer Functions
        //

        void gen_renderbuffers(GLsizei n, GLuint *ids) override;
        void delete_renderbuffers(GLsizei n, GLuint *ids) override;
        void bind_renderbuffer(GLenum target, GLuint renderbuffer) override;
        void renderbuffer_storage(
            GLenum target,
            GLenum internalformat,
            GLsizei width,
            GLsizei height
        ) override;


        //
        // Draw Functions
        //
        void viewport(
            GLint x,
            GLint y,
            GLsizei width,
            GLsizei height
        ) override;
        void draw_elements(
            GLenum mode,
            GLsizei count,
            GLenum type,
            const void *indices
        ) override;

    };
}
