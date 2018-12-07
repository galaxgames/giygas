#pragma once
#include <giygas/EventHandler.hpp>
#include <vulkan/vulkan.h>
#include <memory>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <mutex>

namespace giygas {

    class VulkanRenderer;

    class InUseBufferSafeDeletable;
    typedef Event<InUseBufferSafeDeletable *> SafeDeletableEvent;
    typedef EventHandler<InUseBufferSafeDeletable *> SafeDeletableEventHandler;


    template <VkBufferUsageFlags USAGE_FLAGS>
    class WritableBuffer final {

        VulkanRenderer *_renderer = nullptr;
        VkBuffer _handle = VK_NULL_HANDLE;
        VkDeviceMemory _device_memory = VK_NULL_HANDLE;
        std::vector<uint8_t> _data;
        uint32_t _current_buffer_size = 0;
        std::unordered_map<InUseBufferSafeDeletable *, SafeDeletableEventHandler> _event_handlers;
        std::vector<std::tuple<VkBuffer, VkDeviceMemory, uint32_t>> _available_buffers;
        std::mutex _available_buffers_mutex;
        mutable std::mutex _handle_mutex;

        void handle_buffer_no_longer_in_use(InUseBufferSafeDeletable *deletable);

    public:
        explicit WritableBuffer(VulkanRenderer *renderer);
        WritableBuffer(const WritableBuffer &) = delete;
        WritableBuffer &operator=(const WritableBuffer &) = delete;
        WritableBuffer(WritableBuffer &&) noexcept = delete;
        WritableBuffer &operator=(WritableBuffer &&) noexcept = delete;
        ~WritableBuffer();

        void set_data(uint32_t offset, const uint8_t *data, uint32_t size);
        bool is_valid() const;

        VkBuffer handle() const;

    };

    typedef WritableBuffer<VK_BUFFER_USAGE_VERTEX_BUFFER_BIT> WritableVertexBuffer;
    typedef WritableBuffer<VK_BUFFER_USAGE_INDEX_BUFFER_BIT>  WritableIndexBuffer;

}
