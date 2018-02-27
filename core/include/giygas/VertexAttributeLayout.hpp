#pragma once
#include <cstddef>
#include <vector>
#include <giygas/export.h>

namespace giygas {

    class VertexAttribute {
    public:
        size_t component_count;
        size_t component_size;
        size_t offset;
    };

    class VertexAttributeLayout {
    public:
        size_t stride;
        size_t attribute_count;
        const VertexAttribute *attributes;
    };

}
