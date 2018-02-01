#include <algorithm>
#include "VulkanIndexBuffer.hpp"
#include "VulkanRenderer.hpp"

using namespace giygas;

template <typename T>
VulkanIndexBuffer<T>::VulkanIndexBuffer(VulkanRenderer *renderer) {
    _renderer = renderer;
    _buffer = VK_NULL_HANDLE;
    _device_memory = VK_NULL_HANDLE;
}

template <typename T>
VulkanIndexBuffer<T>::~VulkanIndexBuffer() {
    VkDevice device = _renderer->device();
    vkDestroyBuffer(device, _buffer, nullptr);
    vkFreeMemory(device, _device_memory, nullptr);
}

template <typename T>
RendererType VulkanIndexBuffer<T>::get_renderer_type() const {
    return RendererType::Vulkan;
}

template <typename T>
void VulkanIndexBuffer<T>::set(size_t offset, const T *indices, size_t count) {
    size_t previous_count = _data.size();
    size_t required_count = count + offset;
    size_t required_size = sizeof(T) * count;
    if (previous_count < required_count) {
        _data.resize(required_count);
    }
    copy_n(indices, count, _data.begin() + offset);

    VkDevice device = _renderer->device();

    if (previous_count <= required_count) {
        vkDestroyBuffer(device, _buffer, nullptr);
        VkBufferCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        create_info.size = required_size;
        create_info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        if (vkCreateBuffer(device, &create_info, nullptr, &_buffer) != VK_SUCCESS) {
            return;
        }
        VkMemoryAllocateInfo alloc_info = {};
        alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        alloc_info.allocationSize = required_size;
        alloc_info.memoryTypeIndex = _renderer->find_memory_type(
            _renderer->buffer_memory_requirements().memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );
        if (vkAllocateMemory(device, &alloc_info, nullptr, &_device_memory) != VK_SUCCESS) {
            return;
        }
        vkBindBufferMemory(device, _buffer, _device_memory, 0);
    }

    void *mapped_buffer;
    size_t size = sizeof(T) * count;
    if (vkMapMemory(device, _device_memory, offset, size, 0, &mapped_buffer) != VK_SUCCESS) {
        return;
    }
    std::copy_n(indices, size, static_cast<uint8_t *>(mapped_buffer));
    vkUnmapMemory(device, _device_memory);
}

template <typename T>
size_t VulkanIndexBuffer<T>::count() const {
    return _data.size();
}

namespace giygas {
    template class VulkanIndexBuffer<uint32_t>;
    template class VulkanIndexBuffer<uint16_t>;
    template class VulkanIndexBuffer<uint8_t>;
}