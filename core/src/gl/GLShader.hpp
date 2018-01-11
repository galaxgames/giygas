#pragma once
#include <glad/glad.h>
#include <giygas/Shader.hpp>
#include <string>

namespace giygas {
    using namespace std;

    class GLRenderer;

    class GLShader : public Shader {
        GLRenderer *_renderer;
        GLuint _vertex_shader;
        GLuint _fragment_shader;
        bool _is_valid;
        string _vertex_message;
        string _fragment_message;

        bool compile_shader(GLuint shader, const char *source, string &message);

    public:
        GLShader(GLRenderer *gl);
        GLShader(const GLShader &) = delete;
        GLShader(GLShader &&) noexcept;
        GLShader& operator=(const GLShader &) = delete;
        GLShader& operator=(GLShader &&) noexcept;
        virtual ~GLShader();

        RendererType renderer_type() const override;

        void set_from_source(
            const char* vertex,
            const char* fragment
        ) override;

        bool is_valid() const override;
        const string &get_vertex_message() const override;
        const string &get_fragment_message() const override;

        GLuint get_vertex_shader() const;
        GLuint get_fragment_shader() const;
    };
}
