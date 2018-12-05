#include <algorithm>
#include "VulkanReadOnlyVertexBuffer.hpp"
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


VulkanReadOnlyVertexBuffer::VulkanReadOnlyVertexBuffer(VulkanRenderer *renderer) {
    _renderer = renderer;
}

VulkanReadOnlyVertexBuffer::~VulkanReadOnlyVertexBuffer() {
    _renderer->delete_when_safe(unique_ptr<SwapchainSafeDeleteable>(
        new VertexBufferSafeDeletable(_handle, _device_memory)
    ));
}

RendererType VulkanReadOnlyVertexBuffer::renderer_type() const {
    return RendererType::Vulkan;
}

void VulkanReadOnlyVertexBuffer::set_data(uint32_t offset, const uint8_t *data, uint32_t size) {
    assert(_handle == VK_NULL_HANDLE && "Cannot set data more than once for a read-only vertex buffer");

    VkDevice device = _renderer->device();
    uint32_t total_size = offset + size;

    VkDeviceSize buffer_size = static_cast<VkDeviceSize>(total_size);
    if (buffer_size == 0) {
        buffer_size = 1;
    }

    _renderer->create_buffer(
        buffer_size
        , VK_BUFFER_USAGE_VERTEX_BUFFER_BIT  /* usage */
        , VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT  /* memory_properties */
        , _handle
        , _device_memory
    );

    // Vulkan does not support memory mappings of 0 bytes.
    if (size > 0) {
        void *mapped_buffer;
        if (vkMapMemory(device, _device_memory, offset, size, 0, &mapped_buffer) != VK_SUCCESS) {
            return;
        }
        std::copy_n(data, size, static_cast<uint8_t *>(mapped_buffer));
        vkUnmapMemory(device, _device_memory);
    }
}

bool VulkanReadOnlyVertexBuffer::is_valid() const {
    return _handle != VK_NULL_HANDLE;
}

bool VulkanReadOnlyVertexBuffer::is_writable() const {
    return false;
}

VkBuffer VulkanReadOnlyVertexBuffer::handle() const {
    return _handle;
}
