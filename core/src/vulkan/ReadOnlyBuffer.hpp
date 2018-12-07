#pragma once
#include "VulkanVertexBuffer.hpp"
#include <vulkan/vulkan.h>
#include <memory>

namespace giygas {

    class VulkanRenderer;

    template <VkBufferUsageFlags USAGE_FLAGS>
    class ReadOnlyBuffer final {

        VulkanRenderer *_renderer = nullptr;
        VkBuffer _handle = VK_NULL_HANDLE;
        VkDeviceMemory _device_memory = VK_NULL_HANDLE;

    public:
        explicit ReadOnlyBuffer(VulkanRenderer *renderer);
        ReadOnlyBuffer(const ReadOnlyBuffer &) = delete;
        ReadOnlyBuffer &operator=(const ReadOnlyBuffer &) = delete;
        ReadOnlyBuffer(ReadOnlyBuffer &&) noexcept = delete;
        ReadOnlyBuffer &operator=(ReadOnlyBuffer &&) noexcept = delete;
        ~ReadOnlyBuffer();


        void set_data(uint32_t offset, const uint8_t *data, uint32_t size);
        bool is_valid() const;

        VkBuffer handle() const;

    };

    typedef ReadOnlyBuffer<VK_BUFFER_USAGE_VERTEX_BUFFER_BIT> ReadOnlyVertexBuffer;
    typedef ReadOnlyBuffer<VK_BUFFER_USAGE_INDEX_BUFFER_BIT>  ReadOnlyIndexBuffer;

}
