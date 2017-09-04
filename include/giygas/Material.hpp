#pragma once
#include <memory>
#include <giygas/export.h>
#include <giygas/Shader.hpp>
#include "Texture.hpp"

namespace giygas {
    class GIYGAS_EXPORT Material {
    public:
        virtual ~Material() = default;

        virtual void set_shader(std::weak_ptr<Shader> shader) = 0;

        virtual void set_uniform_float(
            const std::string &name, float value
        ) = 0;

        virtual void set_uniform_texture(
            const std::string &name, std::weak_ptr<Texture> value
        ) = 0;

        virtual bool is_valid() const = 0;
        virtual const char *get_message() const = 0;
    };
}

