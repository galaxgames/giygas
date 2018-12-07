#include <algorithm>
#include "VulkanVertexBufferImpl.hpp"
#include "VulkanRenderer.hpp"

using namespace giygas;


template <typename T>
VulkanVertexBufferImpl<T>::VulkanVertexBufferImpl(VulkanRenderer *renderer)
    : _buffer(renderer)
{
}

template <typename T>
RendererType VulkanVertexBufferImpl<T>::renderer_type() const {
    return RendererType::Vulkan;
}

template <typename T>
void VulkanVertexBufferImpl<T>::set_data(uint32_t offset, const uint8_t *data, uint32_t size) {
    _buffer.set_data(offset, data, size);
}

template <typename T>
bool VulkanVertexBufferImpl<T>::is_valid() const {
    return _buffer.is_valid();
}

template <typename T>
bool VulkanVertexBufferImpl<T>::is_writable() const {
    return true;
}

template <typename T>
VkBuffer VulkanVertexBufferImpl<T>::handle() const {
    return _buffer.handle();
}

namespace giygas {
    template class VulkanVertexBufferImpl<ReadOnlyVertexBuffer>;
    template class VulkanVertexBufferImpl<WritableVertexBuffer>;
}
