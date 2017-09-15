#include <giygas/VertexBufferLayout.hpp>

using namespace giygas;
using namespace std;

void VertexBufferLayout::set_channel(size_t index, size_t size) {
    if (index >= _channels.size()) {
        _channels.resize(index + 1);
    }
    _channels[index] = size;
}

const vector<size_t>& VertexBufferLayout::get_channels() const {
    return _channels;
}
