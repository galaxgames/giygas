#pragma once
#include <memory>
#include <giygas/export.h>
#include <giygas/Shader.hpp>
#include "Texture.hpp"
#include "RendererType.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix4x4.hpp"

#define UNIFORM_FUNC(func_name, T) \
    virtual void func_name(const std::string &name, T value) = 0;

namespace giygas {
    using namespace std;

    class GIYGAS_EXPORT Material {
    public:
        virtual ~Material() = default;

        virtual RendererType renderer_type() const = 0;

        virtual void set_shader(std::weak_ptr<Shader> shader) = 0;

        virtual void set_textures(
            const std::weak_ptr<Texture> *textures, size_t count
        ) = 0;

        UNIFORM_FUNC(set_uniform_float, float)
        UNIFORM_FUNC(set_uniform_texture, size_t)
        UNIFORM_FUNC(set_uniform_vector2, Vector2)
        UNIFORM_FUNC(set_uniform_vector3, Vector3)
        UNIFORM_FUNC(set_uniform_vector4, Vector4)
        UNIFORM_FUNC(set_uniform_matrix4x4, const Matrix4x4 &)

        virtual bool is_valid() const = 0;
        virtual const string &get_message() const = 0;
    };
}

#undef UNIFORM_FUNC
