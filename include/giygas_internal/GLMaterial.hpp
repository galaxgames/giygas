#pragma once
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <giygas/Material.hpp>
#include <giygas/Texture.hpp>
#include <giygas_internal/GL.hpp>
#include "GLShader.hpp"

#define UNIFORM_FUNC(func_name, T) \
    void func_name(const std::string &name, T value) override;

#define UNIFORM_VALUE_CLASS(class_name, T) \
    class class_name : public UniformValue { \
        T _value; \
    public: \
        class_name(T value); \
        void do_gl_call(GL *gl, GLint location) override; \
    }

namespace giygas {

    class UniformValue;

    class GLMaterial : public Material {
        GL *_gl;
        GLuint _program;
        std::weak_ptr<Shader> _shader;
        std::unordered_map<std::string, std::unique_ptr<UniformValue>> _values;
        std::unordered_map<std::string, GLint> _locations;
        std::unique_ptr<std::weak_ptr<Texture>[]> _textures;
        size_t _texture_count;
        bool _is_valid;
        char *_message;

        GLint get_location(const std::string& name) const;

        template <typename T> 
        UniformValue* make_value(T value);

        template <typename T>
        void set_uniform(const std::string &name, T value);

    public:
        GLMaterial(GL *gl);
        GLMaterial(const GLMaterial &) = delete;
        GLMaterial(GLMaterial &&) noexcept;
        GLMaterial& operator=(const GLMaterial &) = delete;
        GLMaterial& operator=(GLMaterial &&) noexcept ;
        virtual ~GLMaterial();

        RendererType renderer_type() const override;

        void set_shader(std::weak_ptr<Shader> shader) override;
        
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
        const char *get_message() const override;

        GLint get_cached_location(const std::string& name) const;
        GLuint get_program() const;
        size_t get_texture_count() const;
        std::weak_ptr<Texture> get_texture(size_t i) const;

    };

    // *****
    // TODO: Move these classes into their own files
    // *****

    class UniformValue {
    public:
        virtual ~UniformValue() = default;
        virtual void do_gl_call(GL *gl, GLint location) = 0;
    };

    UNIFORM_VALUE_CLASS(FloatUniformValue, float);
    UNIFORM_VALUE_CLASS(IntUniformValue, int);
    UNIFORM_VALUE_CLASS(Vector2UniformValue, Vector2);
    UNIFORM_VALUE_CLASS(Vector3UniformValue, Vector3);
    UNIFORM_VALUE_CLASS(Vector4UniformValue, Vector4);

    class Matrix4x4UniformValue : public UniformValue {
        Matrix4x4 _value;
    public:
        Matrix4x4UniformValue(const Matrix4x4 &value);
        void do_gl_call(GL *gl, GLint location) override;
    };

}

#undef UNIFORM_FUNC
