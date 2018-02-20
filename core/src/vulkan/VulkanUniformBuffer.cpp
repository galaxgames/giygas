#include <algorithm>
#include "VulkanUniformBuffer.hpp"
#include "VulkanRenderer.hpp"

using namespace giygas;

VulkanUniformBuffer::VulkanUniformBuffer(VulkanRenderer *renderer) {
    _renderer = renderer;
    _handle = VK_NULL_HANDLE;
    _device_memory = VK_NULL_HANDLE;
    _mapped_buffer = nullptr;
}

VulkanUniformBuffer::~VulkanUniformBuffer() {
    VkDevice device = _renderer->device();
    if (_device_memory != VK_NULL_HANDLE) {
        vkUnmapMemory(device, _device_memory);
    }
    vkFreeMemory(device, _device_memory, nullptr);
    vkDestroyBuffer(device, _handle, nullptr);
}

RendererType VulkanUniformBuffer::renderer_type() const {
    return RendererType::Vulkan;
}

void VulkanUniformBuffer::set_data(size_t offset, const uint8_t *data, size_t size) {
    VkDevice device = _renderer->device();
    size_t required_size = offset + size;
    bool needs_new_buffer = _data.size() < required_size;

    if (needs_new_buffer) {
        size_t previous_size = _data.size();
        _data.resize(required_size);
        if (_device_memory != VK_NULL_HANDLE) {
            vkUnmapMemory(device, _device_memory);
        }
        vkFreeMemory(device, _device_memory, nullptr);
        vkDestroyBuffer(device, _handle, nullptr);
        _renderer->create_buffer(
            static_cast<VkDeviceSize>(required_size),
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            _handle,
            _device_memory
        );

        vkMapMemory(device, _device_memory, offset, size, 0, &_mapped_buffer);

        if (_mapped_buffer != nullptr) {
            copy_n(_data.data(), previous_size, static_cast<uint8_t *>(_mapped_buffer));
        }
    }

    copy_n(data, size, _data.data() + offset);

    if (_mapped_buffer == nullptr) {
        // TODO: Should probably warn about this?
        return;
    }
    copy_n(data, size, static_cast<uint8_t *>(_mapped_buffer));
}

VkBuffer VulkanUniformBuffer::handle() const {
    return _handle;
}

size_t VulkanUniformBuffer::size() const {
    return _data.size();
}
