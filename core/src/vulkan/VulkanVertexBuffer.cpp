#include <algorithm>
#include "VulkanVertexBuffer.hpp"
#include "VulkanRenderer.hpp"

using namespace giygas;

VulkanVertexBuffer::VulkanVertexBuffer(VulkanRenderer *renderer) {
    _renderer = renderer;
    _handle = VK_NULL_HANDLE;
    _device_memory = VK_NULL_HANDLE;
}

VulkanVertexBuffer::~VulkanVertexBuffer() {
    VkDevice device = _renderer->device();
    vkDestroyBuffer(device, _handle, nullptr);
    vkFreeMemory(device, _device_memory, nullptr);
}

RendererType VulkanVertexBuffer::get_renderer_type() const {
    return RendererType::Vulkan;
}

void VulkanVertexBuffer::set_data(size_t offset, const uint8_t *data, size_t size) {
    size_t previous_size = _data.size();
    size_t required_size = size + offset;
    if (previous_size < required_size) {
        _data.resize(required_size);
    }
    copy_n(data, size, _data.begin() + offset);

    VkDevice device = _renderer->device();

    if (previous_size <= required_size) {
        vkDestroyBuffer(device, _handle, nullptr);
        VkBufferCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        create_info.size = sizeof(uint8_t) * required_size;
        create_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        if (vkCreateBuffer(device, &create_info, nullptr, &_handle) != VK_SUCCESS) {
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
        vkBindBufferMemory(device, _handle, _device_memory, 0);
    }

    void *mapped_buffer;
    if (vkMapMemory(device, _device_memory, offset, size, 0, &mapped_buffer) != VK_SUCCESS) {
        return;
    }
    std::copy_n(data, size, static_cast<uint8_t *>(mapped_buffer));
    vkUnmapMemory(device, _device_memory);
}
