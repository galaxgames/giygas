#pragma once
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <giygas/Material.hpp>
#include <giygas/Texture.hpp>
#include "GL.hpp"
#include "GLShader.hpp"

#define UNIFORM_FUNC(func_name, T) \
    void func_name(const std::string &name, T value) override;


namespace giygas {
    using namespace std;

    class GLRenderer;
    class GLUniformValue;

    class GLMaterial : public Material {
        GLRenderer *_renderer;
        GLuint _program;
        //std::weak_ptr<Shader> _shader;
        std::unordered_map<std::string, std::unique_ptr<GLUniformValue>> _values;
        std::unordered_map<std::string, GLint> _locations;
        std::unique_ptr<std::weak_ptr<Texture>[]> _textures;
        size_t _texture_count;
        bool _is_valid;
        string _message;

        void move_common(GLMaterial &&other) noexcept;
        GLint get_location(const std::string& name) const;

        template <typename T> 
        GLUniformValue* make_value(T value);

        template <typename T>
        void set_uniform(const std::string &name, T value);

    public:
        explicit GLMaterial(GLRenderer *renderer);
        GLMaterial(const GLMaterial &) = delete;
        GLMaterial(GLMaterial &&) noexcept;
        GLMaterial& operator=(const GLMaterial &) = delete;
        GLMaterial& operator=(GLMaterial &&) noexcept ;
        ~GLMaterial() override;

        RendererType renderer_type() const override;

        void link_shaders(const Shader **shaders, size_t count) override;
        //void set_shader(std::weak_ptr<Shader> shader) override;
        
        void set_textures(
            const std::weak_ptr<Texture> *textures, size_t count
        ) override;

        UNIFORM_FUNC(set_uniform_float, float)
        UNIFORM_FUNC(set_uniform_texture, size_t)
        UNIFORM_FUNC(set_uniform_vector2, Vector2)
        UNIFORM_FUNC(set_uniform_vector3, Vector3)
        UNIFORM_FUNC(set_uniform_vector4, Vector4)
        UNIFORM_FUNC(set_uniform_matrix4x4, const Matrix4x4 &)

        bool is_valid() const override;
        const string &get_message() const override;

        GLint get_cached_location(const std::string& name) const;
        GLuint get_program() const;
        size_t get_texture_count() const;
        std::weak_ptr<Texture> get_texture(size_t i) const;

    };

}

#undef UNIFORM_FUNC
