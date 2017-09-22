#pragma once
#include <cstddef>
#include <vector>
#include <giygas/export.h>

namespace giygas {

    class GIYGAS_EXPORT LayoutAttribute {
    public:
        size_t component_count;
        size_t component_size;
    };

    class GIYGAS_EXPORT VertexBufferLayout {
        std::vector<LayoutAttribute> _attributes;

    public:
        VertexBufferLayout() = default;
        VertexBufferLayout(size_t attribute_count_hint);
        VertexBufferLayout(const VertexBufferLayout &) = delete;
        VertexBufferLayout& operator=(const VertexBufferLayout &) = delete;
        VertexBufferLayout(VertexBufferLayout &&) = default;
        VertexBufferLayout& operator=(VertexBufferLayout &&) = default;
        virtual ~VertexBufferLayout() = default;

        void add_attribute(LayoutAttribute attrib);
        void add_attribute(size_t component_count, size_t component_size);
        const std::vector<LayoutAttribute>& attributes() const;
    };

}
