#include <giygas/VertexAttributeLayout.hpp>

using namespace giygas;
using namespace std;

VertexAttributeLayout::VertexAttributeLayout() {
    stride = 0;
}

VertexAttributeLayout::VertexAttributeLayout(
    size_t attribute_count_hint, size_t stride
) {
    _attributes.reserve(attribute_count_hint);
    this->stride = stride;
}

void VertexAttributeLayout::add_attribute(
    size_t component_count, size_t component_size, size_t offset
) {
    LayoutAttribute attrib;
    attrib.component_size = component_size;
    attrib.component_count = component_count;
    attrib.offset = offset;
    add_attribute(attrib);
}

void VertexAttributeLayout::add_attribute(LayoutAttribute attrib) {
    _attributes.push_back(attrib);
}

const vector<LayoutAttribute>& VertexAttributeLayout::attributes() const {
    return _attributes;
}
