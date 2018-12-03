#include <algorithm>
#include "VulkanVertexBuffer.hpp"
#include "VulkanRenderer.hpp"

using namespace giygas;

class VertexBufferSafeDeletable final : public SwapchainSafeDeleteable {

    VkBuffer _handle;
    VkDeviceMemory _device_memory;

public:

    VertexBufferSafeDeletable(VkBuffer handle, VkDeviceMemory device_memory) {
        _handle = handle;
        _device_memory = device_memory;
    }

    void delete_resources(VulkanRenderer &renderer) {
        VkDevice device = renderer.device();
        vkDestroyBuffer(device, _handle, nullptr);
        vkFreeMemory(device, _device_memory, nullptr);
    }

};


VulkanVertexBuffer::VulkanVertexBuffer(VulkanRenderer *renderer) {
    _renderer = renderer;
    _handle = VK_NULL_HANDLE;
    _device_memory = VK_NULL_HANDLE;
}

VulkanVertexBuffer::~VulkanVertexBuffer() {
    safe_delete_resources();
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

    // set_data ensures that the buffer if valid and ready for usage, so let's make an interal
    // buffer is created for zero-sized buffers.
    if (previous_size < required_size || _handle == VK_NULL_HANDLE) {
        safe_delete_resources();

        VkDeviceSize buffer_size = static_cast<VkDeviceSize>(required_size);
        if (buffer_size == 0) {
            buffer_size = 1;
        }

        _renderer->create_buffer(
            buffer_size,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            _handle,
            _device_memory
        );
    }

    size_t copy_size = size;
    size_t copy_offset = offset;
    if (previous_size < required_size) {
        copy_size = required_size;
        copy_offset = 0;
    }

    // Vulkan does not support mapping 0-sized buffers.
    if (copy_size > 0) {
        void *mapped_buffer;
        if (vkMapMemory(device, _device_memory, copy_offset, copy_size, 0, &mapped_buffer) != VK_SUCCESS) {
            return;
        }
        std::copy_n(data, size, static_cast<uint8_t *>(mapped_buffer));
        vkUnmapMemory(device, _device_memory);
    }
}

bool VulkanVertexBuffer::is_valid() const {
    return _handle != VK_NULL_HANDLE;
}

VkBuffer VulkanVertexBuffer::handle() const {
    return _handle;
}

void VulkanVertexBuffer::safe_delete_resources() {
    _renderer->delete_when_safe(unique_ptr<SwapchainSafeDeleteable>(
        new VertexBufferSafeDeletable(_handle, _device_memory)
    ));
}
