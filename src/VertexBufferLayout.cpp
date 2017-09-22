#include <giygas/VertexBufferLayout.hpp>

using namespace giygas;
using namespace std;

VertexBufferLayout::VertexBufferLayout(size_t attribute_count_hint) {
    _attributes.reserve(attribute_count_hint);
}

void VertexBufferLayout::add_attribute(
    size_t component_count, size_t component_size
) {
    LayoutAttribute attrib;
    attrib.component_size = component_size;
    attrib.component_count = component_count;
    add_attribute(attrib);
}

void VertexBufferLayout::add_attribute(LayoutAttribute attrib) {
    _attributes.push_back(attrib);
}

const vector<LayoutAttribute>& VertexBufferLayout::attributes() const {
    return _attributes;
}
