#pragma once
#include <giygas/export.h>
#include "RendererType.hpp"
#include "ShaderType.hpp"
#include <string>

namespace giygas {
    using namespace std;

    class GIYGAS_EXPORT Shader {
    public:
        virtual ~Shader() = default;

        virtual RendererType renderer_type() const = 0;

        virtual void set_code(
            const uint8_t* code, size_t length, ShaderType type
        ) = 0;

        virtual ShaderType shader_type() const = 0;

        //virtual bool is_valid() const = 0;
        //virtual const string &get_vertex_message() const = 0;
        //virtual const string &get_fragment_message() const = 0;

    };
}
