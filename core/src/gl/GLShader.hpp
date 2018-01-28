#pragma once
#include <glad/glad.h>
#include <giygas/Shader.hpp>
#include <giygas/ShaderType.hpp>
#include <string>

namespace giygas {
    using namespace std;

    class GLRenderer;

    class GLShader : public Shader {
        GLRenderer *_renderer;
        GLuint _handle;
        ShaderType _type;

        void create_shader(ShaderType type);
        void delete_shader();
        static GLenum shader_type_to_enum(ShaderType type);

    public:
        explicit GLShader(GLRenderer *gl);
        GLShader(const GLShader &) = delete;
        GLShader(GLShader &&) noexcept;
        GLShader& operator=(const GLShader &) = delete;
        GLShader& operator=(GLShader &&) noexcept;
        ~GLShader() override;

        RendererType renderer_type() const override;

        void set_code(
            const uint8_t* vertex, size_t length, ShaderType type
        ) override;

        ShaderType shader_type() const override;

//        bool is_valid() const override;
//        const string &get_vertex_message() const override;
//        const string &get_fragment_message() const override;

//        GLuint get_vertex_shader() const;
//        GLuint get_fragment_shader() const;

        GLuint handle() const;
    };
}
