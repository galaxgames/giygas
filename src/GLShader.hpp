#pragma once
#include <glad/glad.h>
#include <giygas/Shader.hpp>

namespace giygas {
    class GLShader : public Shader {
        GLuint _vertex_shader;
        GLuint _fragment_shader;
        bool _is_valid;
        char *_vertex_message;
        char *_fragment_message;

        static bool compile_shader(
            GLuint shader, const char *source, char **message
        );

    public:
        GLShader();
        GLShader(const GLShader &) = delete;
        GLShader(GLShader &&) noexcept;
        GLShader& operator=(const GLShader &) = delete;
        GLShader& operator=(const GLShader &&) noexcept;
        virtual ~GLShader();

        void set_from_source(
            const char* vertex,
            const char* fragment
        ) override;

        bool is_valid() const override;
        const char *get_vertex_message() const override;
        const char *get_fragment_message() const override;

        GLuint get_vertex_shader() const;
        GLuint get_fragment_shader() const;
    };
}