#pragma once
#include <giygas/export.h>
#include "RendererType.hpp"
#include "ShaderType.hpp"
#include <string>

namespace giygas {

    class GIYGAS_EXPORT Shader {
    public:
        virtual ~Shader() = default;

        virtual RendererType renderer_type() const = 0;

        virtual void set_code(
            const uint8_t* code, uint32_t length, ShaderType type
        ) = 0;

        virtual ShaderType shader_type() const = 0;
    };
}
