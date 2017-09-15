#pragma once
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <giygas/Material.hpp>
#include <giygas/Texture.hpp>
#include <giygas/GL.hpp>
#include "GLShader.hpp"

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

        void set_uniform_float(const std::string &name, float value) override;
        void set_uniform_texture(
                const std::string &name, size_t index
        ) override;

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

    class FloatUniformValue : public UniformValue {
        float _value;
    public:
        FloatUniformValue(float value);
        void do_gl_call(GL *gl, GLint location) override;
    };

    class IntUniformValue : public UniformValue {
        int _value;
    public:
        IntUniformValue(int value);
        void do_gl_call(GL *gl, GLint location) override;
    };
        
}
