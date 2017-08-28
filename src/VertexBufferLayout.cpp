#include <giygas/VertexBufferLayout.hpp>

using namespace giygas;
using namespace std;

void VertexBufferLayout::set_channel(int index, int size) {
    if (index >= _channels.size()) {
        _channels.resize(index + 1);
    }
    _channels[index] = size;
}

const vector<int>& VertexBufferLayout::get_channels() const {
    return _channels;
}