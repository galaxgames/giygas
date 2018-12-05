#pragma once
#include "VulkanVertexBuffer.hpp"
#include <giygas/EventHandler.hpp>
#include <vulkan/vulkan.h>
#include <memory>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <thread>

namespace giygas {
    using namespace std;

    class VulkanRenderer;

    class InUseVertexBufferSafeDeletable;
    typedef Event<InUseVertexBufferSafeDeletable *, VkBuffer, VkDeviceMemory> SafeDeletableEvent;
    typedef EventHandler<InUseVertexBufferSafeDeletable *, VkBuffer, VkDeviceMemory> SafeDeletableEventHandler;


    class VulkanWritableVertexBuffer final : public VulkanVertexBuffer {

        VulkanRenderer *_renderer = nullptr;
        VkBuffer _handle = VK_NULL_HANDLE;
        VkDeviceMemory _device_memory = VK_NULL_HANDLE;
        vector<uint8_t> _data;
        std::unordered_map<InUseVertexBufferSafeDeletable *, SafeDeletableEventHandler> _event_handlers;
        std::vector<std::tuple<VkBuffer, VkDeviceMemory>> _available_buffers;
        std::mutex _available_buffers_mutex;
        mutable std::mutex _handle_mutex;

        void safe_delete_resources();
        void handle_buffer_no_longer_in_use(InUseVertexBufferSafeDeletable *deletable, VkBuffer buffer, VkDeviceMemory memory);

    public:
        explicit VulkanWritableVertexBuffer(VulkanRenderer *renderer);
        VulkanWritableVertexBuffer(const VulkanWritableVertexBuffer &) = delete;
        VulkanWritableVertexBuffer &operator=(const VulkanWritableVertexBuffer &) = delete;
        VulkanWritableVertexBuffer(VulkanWritableVertexBuffer &&) noexcept = delete;
        VulkanWritableVertexBuffer &operator=(VulkanWritableVertexBuffer &&) noexcept = delete;
        ~VulkanWritableVertexBuffer() override;

        //
        // VertexBuffer implementation
        //

        RendererType renderer_type() const override;
        void *cast_to_renderer_specific() override;

        void set_data(uint32_t offset, const uint8_t *data, uint32_t size) override;
        bool is_valid() const override;
        bool is_writable() const override;

        //
        // VulkanVertexBuffer implementation
        //

        VkBuffer handle() const override;

    };

}
