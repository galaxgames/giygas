#include <algorithm>
#include "VulkanIndexBuffer.hpp"
#include "VulkanRenderer.hpp"

using namespace giygas;

template <typename T, typename DeviceT>
VulkanIndexBuffer<T, DeviceT>::VulkanIndexBuffer(VulkanRenderer *renderer) {
    _renderer = renderer;
    _buffer = VK_NULL_HANDLE;
    _device_memory = VK_NULL_HANDLE;
}

template <typename T, typename DeviceT>
VulkanIndexBuffer<T, DeviceT>::~VulkanIndexBuffer() {
    VkDevice device = _renderer->device();
    vkDestroyBuffer(device, _buffer, nullptr);
    vkFreeMemory(device, _device_memory, nullptr);
}

template <typename T, typename DeviceT>
RendererType VulkanIndexBuffer<T, DeviceT>::renderer_type() const {
    return RendererType::Vulkan;
}

template <typename T, typename DeviceT>
const void* VulkanIndexBuffer<T, DeviceT>::cast_to_specific() const {
    return static_cast<const VulkanGenericIndexBuffer *>(this);
}

template <typename T, typename DeviceT>
void VulkanIndexBuffer<T, DeviceT>::set(size_t offset, const T *indices, size_t count) {
    size_t previous_count = _data.size();
    size_t required_count = count + offset;
    size_t required_device_size = sizeof(DeviceT) * required_count;
    bool needs_new_buffer = previous_count < required_count;
    if (needs_new_buffer) {
        _data.resize(required_count);
    }
    copy_n(indices, count, _data.begin() + offset);

    VkDevice device = _renderer->device();

    // set_data ensures that the buffer if valid and ready for usage, so let's make an interal
    // buffer is created for zero-sized buffers.
    if (needs_new_buffer || _buffer == VK_NULL_HANDLE) {
        vkFreeMemory(device, _device_memory, nullptr);
        vkDestroyBuffer(device, _buffer, nullptr);

        VkDeviceSize buffer_size = static_cast<VkDeviceSize>(required_device_size);
        if (buffer_size == 0) {
            buffer_size = 1;
        }

        _renderer->create_buffer(
            buffer_size,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            _buffer,
            _device_memory
        );
    }

    void *mapped_buffer;

    size_t copy_offset;
    size_t copy_count;
    if (needs_new_buffer) {
        copy_offset = 0;
        copy_count = required_count;
    }
    else {
        copy_offset = offset;
        copy_count = count;
    }

    size_t device_offset = sizeof(DeviceT) * copy_offset;
    size_t device_size = sizeof(DeviceT) * copy_count;

    // Vulkan does not support buffer mappings of 0 size.
    if (device_size == 0) {
        return;
    }

    if (vkMapMemory(device, _device_memory, device_offset, device_size, 0, &mapped_buffer) != VK_SUCCESS) {
        return;
    }

    DeviceT *typed_buffer = static_cast<DeviceT *>(mapped_buffer);

    for (size_t i = copy_offset; i < copy_count; ++i) {
        typed_buffer[i] = static_cast<DeviceT>(_data[i]);
    }

    vkUnmapMemory(device, _device_memory);
}

template <typename T, typename DeviceT>
size_t VulkanIndexBuffer<T, DeviceT>::count() const {
    return _data.size();
}

template <typename T, typename DeviceT>
VkBuffer VulkanIndexBuffer<T, DeviceT>::handle() const {
    return _buffer;
}

template <>
VkIndexType VulkanIndexBuffer<uint32_t, uint32_t>::index_type() const {
    return VK_INDEX_TYPE_UINT32;
}

template <>
VkIndexType VulkanIndexBuffer<uint16_t, uint16_t>::index_type() const {
    return VK_INDEX_TYPE_UINT16;
}

template <>
VkIndexType VulkanIndexBuffer<uint8_t, uint16_t>::index_type() const {
    return VK_INDEX_TYPE_UINT16;
}

namespace giygas {
    template class VulkanIndexBuffer<uint32_t, uint32_t>;
    template class VulkanIndexBuffer<uint16_t, uint16_t>;
    template class VulkanIndexBuffer<uint8_t, uint16_t>;
}