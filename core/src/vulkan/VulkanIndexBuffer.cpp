#include <algorithm>
#include "VulkanIndexBuffer.hpp"
#include "VulkanRenderer.hpp"

using namespace std;
using namespace giygas;

template <typename T, typename DeviceT, typename BufferT>
VulkanIndexBuffer<T, DeviceT, BufferT>::VulkanIndexBuffer(VulkanRenderer *renderer)
    : _buffer(renderer)
{
}

template <typename T, typename DeviceT, typename BufferT>
RendererType VulkanIndexBuffer<T, DeviceT, BufferT>::renderer_type() const {
    return RendererType::Vulkan;
}

template <typename T, typename DeviceT, typename BufferT>
const void* VulkanIndexBuffer<T, DeviceT, BufferT>::cast_to_specific() const {
    return static_cast<const VulkanGenericIndexBuffer *>(this);
}

template <typename T, typename DeviceT, typename BufferT>
void VulkanIndexBuffer<T, DeviceT, BufferT>::set(uint32_t offset, const T *indices, uint32_t count) {
    if (is_same<T, DeviceT>::value) {
        _buffer.set_data(offset * sizeof(DeviceT), reinterpret_cast<const uint8_t *>(indices), count * sizeof(DeviceT));
    } else {
        // TODO: Reduce allocations of this buffer
        unique_ptr<DeviceT []> converted_data(new DeviceT[count]);
        copy_n(indices, count, converted_data.get());
        _buffer.set_data(offset * sizeof(DeviceT), reinterpret_cast<const uint8_t *>(converted_data.get()), count * sizeof(DeviceT));
    }
    _count = max(_count, offset + count);
}

template <typename T, typename DeviceT, typename BufferT>
size_t VulkanIndexBuffer<T, DeviceT, BufferT>::count() const {
    return _count;
}

template <typename T, typename DeviceT, typename BufferT>
VkBuffer VulkanIndexBuffer<T, DeviceT, BufferT>::handle() const {
    return _buffer.handle();
}

template <>
VkIndexType VulkanIndexBuffer<uint32_t, uint32_t, ReadOnlyIndexBuffer>::index_type() const {
    return VK_INDEX_TYPE_UINT32;
}

template <>
VkIndexType VulkanIndexBuffer<uint16_t, uint16_t, ReadOnlyIndexBuffer>::index_type() const {
    return VK_INDEX_TYPE_UINT16;
}

template <>
VkIndexType VulkanIndexBuffer<uint8_t, uint16_t, ReadOnlyIndexBuffer>::index_type() const {
    return VK_INDEX_TYPE_UINT16;
}

template <>
VkIndexType VulkanIndexBuffer<uint32_t, uint32_t, WritableIndexBuffer>::index_type() const {
    return VK_INDEX_TYPE_UINT32;
}

template <>
VkIndexType VulkanIndexBuffer<uint16_t, uint16_t, WritableIndexBuffer>::index_type() const {
    return VK_INDEX_TYPE_UINT16;
}

template <>
VkIndexType VulkanIndexBuffer<uint8_t, uint16_t, WritableIndexBuffer>::index_type() const {
    return VK_INDEX_TYPE_UINT16;
}

namespace giygas {
    template class VulkanIndexBuffer<uint32_t, uint32_t, ReadOnlyIndexBuffer>;
    template class VulkanIndexBuffer<uint16_t, uint16_t, ReadOnlyIndexBuffer>;
    template class VulkanIndexBuffer<uint8_t, uint16_t, ReadOnlyIndexBuffer>;

    template class VulkanIndexBuffer<uint32_t, uint32_t, WritableIndexBuffer>;
    template class VulkanIndexBuffer<uint16_t, uint16_t, WritableIndexBuffer>;
    template class VulkanIndexBuffer<uint8_t, uint16_t, WritableIndexBuffer>;
}
