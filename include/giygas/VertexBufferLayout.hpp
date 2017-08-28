#pragma once
#include <vector>
#include <giygas/export.h>

namespace giygas {
    class GIYGAS_EXPORT VertexBufferLayout {
        std::vector<int> _channels;

    public:
        VertexBufferLayout() = default;
        VertexBufferLayout(const VertexBufferLayout &) = delete;
        VertexBufferLayout& operator=(const VertexBufferLayout &) = delete;
        VertexBufferLayout(VertexBufferLayout &&) = default;
        VertexBufferLayout& operator=(VertexBufferLayout &&) = default;
        virtual ~VertexBufferLayout() = default;

        void set_channel(int index, int size);
        const std::vector<int>& get_channels() const;
    };

}