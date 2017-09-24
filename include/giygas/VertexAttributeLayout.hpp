#pragma once
#include <cstddef>
#include <vector>
#include <giygas/export.h>

namespace giygas {

    class GIYGAS_EXPORT LayoutAttribute {
    public:
        size_t component_count;
        size_t component_size;
        size_t offset;
    };

    class GIYGAS_EXPORT VertexAttributeLayout {
        std::vector<LayoutAttribute> _attributes;

    public:
        size_t stride;

        VertexAttributeLayout();
        VertexAttributeLayout(size_t attribute_count_hint, size_t stride);
        VertexAttributeLayout(const VertexAttributeLayout &) = delete;
        VertexAttributeLayout& operator=(const VertexAttributeLayout &) = delete;
        VertexAttributeLayout(VertexAttributeLayout &&) = default;
        VertexAttributeLayout& operator=(VertexAttributeLayout &&) = default;
        virtual ~VertexAttributeLayout() = default;

        void add_attribute(LayoutAttribute attrib);
        void add_attribute(
            size_t component_count, size_t component_size, size_t offset
        );
        const std::vector<LayoutAttribute>& attributes() const;
    };

}
