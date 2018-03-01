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

RendererType VulkanVertexBuffer::renderer_type() const {
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
        vkFreeMemory(device, _device_memory, nullptr);
        vkDestroyBuffer(device, _handle, nullptr);
        _renderer->create_buffer(
            static_cast<VkDeviceSize>(required_size),
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            _handle,
            _device_memory
        );
    }

    void *mapped_buffer;
    if (vkMapMemory(device, _device_memory, offset, size, 0, &mapped_buffer) != VK_SUCCESS) {
        return;
    }
    std::copy_n(data, size, static_cast<uint8_t *>(mapped_buffer));
    vkUnmapMemory(device, _device_memory);
}

bool VulkanVertexBuffer::is_valid() const {
    return _handle != VK_NULL_HANDLE;
}

VkBuffer VulkanVertexBuffer::handle() const {
    return _handle;
}
