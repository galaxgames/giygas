#pragma once
#include <giygas/export.h>
#include "RendererType.hpp"
#include <string>

namespace giygas {
    using namespace std;

    class GIYGAS_EXPORT Shader {
    public:
        virtual ~Shader() = default;

        virtual RendererType renderer_type() const = 0;

        virtual void set_from_source(
            const char* vertex, const char* fragment
        ) = 0;

        virtual bool is_valid() const = 0;
        virtual const string &get_vertex_message() const = 0;
        virtual const string &get_fragment_message() const = 0;

    };
}
