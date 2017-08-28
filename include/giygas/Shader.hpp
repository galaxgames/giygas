#pragma once
#include <giygas/export.h>

namespace giygas {
    class GIYGAS_EXPORT Shader {
    public:
        virtual void set_from_source(
            const char* vertex, const char* fragment
        ) = 0;

        virtual bool is_valid() const = 0;
        virtual const char *get_vertex_message() const = 0;
        virtual const char *get_fragment_message() const = 0;

    };
}