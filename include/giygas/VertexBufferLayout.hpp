#pragma once
#include <cstddef>
#include <vector>
#include <giygas/export.h>

namespace giygas {
    class GIYGAS_EXPORT VertexBufferLayout {
        std::vector<size_t> _channels;

    public:
        VertexBufferLayout() = default;
        VertexBufferLayout(const VertexBufferLayout &) = delete;
        VertexBufferLayout& operator=(const VertexBufferLayout &) = delete;
        VertexBufferLayout(VertexBufferLayout &&) = default;
        VertexBufferLayout& operator=(VertexBufferLayout &&) = default;
        virtual ~VertexBufferLayout() = default;

        void set_channel(size_t index, size_t size);
        const std::vector<size_t>& get_channels() const;
    };

}
