#include <algorithm>
#include "WritableBuffer.hpp"
#include "VulkanRenderer.hpp"

#define GIYGAS_EVENT_INCLUDE_HELPERS
#include <giygas/EventHandler.hpp>

using namespace std;
using namespace giygas;

#define TEMPLATE template <VkBufferUsageFlags USAGE_FLAGS>
#define CLASS WritableBuffer<USAGE_FLAGS>

namespace giygas {

    class InUseBufferSafeDeletable final : public SwapchainSafeDeleteable {

        SafeDeletableEvent _event;
        VkBuffer _buffer = VK_NULL_HANDLE;
        VkDeviceMemory _memory = VK_NULL_HANDLE;

    public:

        InUseBufferSafeDeletable(InUseBufferSafeDeletable &&) = default;
        InUseBufferSafeDeletable &operator=(InUseBufferSafeDeletable &&) = default;

        InUseBufferSafeDeletable(VkBuffer buffer, VkDeviceMemory memory) {
            _buffer = buffer;
            _memory = memory;
        }

        void delete_resources(VulkanRenderer &renderer) override {
            if (_event.has_handlers()) {
                _event.invoke(this, _buffer, _memory);
            } else {
                VkDevice device = renderer.device();
                vkFreeMemory(device, _memory, nullptr);
                vkDestroyBuffer(device, _buffer, nullptr);
            }
        }

        SafeDeletableEventHandler resources_no_longer_in_use() {
            return _event.make_handler();
        }

    };

}


TEMPLATE CLASS::WritableBuffer(VulkanRenderer *renderer) {
    _renderer = renderer;
}

TEMPLATE CLASS::~WritableBuffer() {
    _renderer->delete_when_safe(unique_ptr<SwapchainSafeDeleteable>(
        new InUseBufferSafeDeletable(_handle, _device_memory)
    ));
    { lock_guard<mutex> _(_available_buffers_mutex);
        for (tuple<VkBuffer, VkDeviceMemory> buffer_and_memory : _available_buffers) {
            _renderer->delete_when_safe(unique_ptr<SwapchainSafeDeleteable>(
                new InUseBufferSafeDeletable(get<0>(buffer_and_memory), get<1>(buffer_and_memory))
            ));
        }
        _event_handlers.clear();
    }
}

TEMPLATE
void CLASS::set_data(uint32_t offset, const uint8_t *data, uint32_t size) {
    uint32_t previous_size = _data.size();
    uint32_t required_size = size + offset;
    if (previous_size < required_size) {
        _data.resize(required_size);
    }
    copy_n(data, size, _data.begin() + offset);


    VkDevice device = _renderer->device();
    VkDeviceSize buffer_size = static_cast<VkDeviceSize>(required_size);
    if (buffer_size == 0) {
        buffer_size = 1;
    }

    VkBuffer buffer = _handle;
    VkDeviceMemory memory = _device_memory;

    { lock_guard<mutex> _(_available_buffers_mutex);

        if (buffer != VK_NULL_HANDLE) {
            // Return current buffer that might be in use
            auto *in_use_deletable = new InUseBufferSafeDeletable(buffer, memory);
            auto no_longer_in_use_handler = in_use_deletable->resources_no_longer_in_use();
            no_longer_in_use_handler.delegate = BIND_MEMBER3(&WritableBuffer::handle_buffer_no_longer_in_use);
            _event_handlers.emplace(
                piecewise_construct
                , forward_as_tuple(in_use_deletable)
                , forward_as_tuple(move(no_longer_in_use_handler))
            );
            _renderer->delete_when_safe(unique_ptr<SwapchainSafeDeleteable>(in_use_deletable));
            buffer = VK_NULL_HANDLE;
            memory = VK_NULL_HANDLE;
        }

        // Get a buffer ready to be written.
        if (!_available_buffers.empty()) {
            tuple<VkBuffer, VkDeviceMemory> buffer_and_memory =  _available_buffers.back();
            buffer = get<0>(buffer_and_memory);
            memory = get<1>(buffer_and_memory);
            _available_buffers.pop_back();
        }
    }

    if (buffer == VK_NULL_HANDLE) {
        // Need to create a new buffer
        // TODO: Less copy + paste
        _renderer->create_buffer(
            buffer_size
            , USAGE_FLAGS  /* usage */
            , VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT  /* memory_properties */
            , buffer
            , memory
        );
    }

    // Vulkan does not support memory mappings of 0 bytes.
    if (size > 0) {
        void *mapped_buffer;
        if (vkMapMemory(device, memory, 0, required_size, 0, &mapped_buffer) != VK_SUCCESS) {
            return;
        }
        std::copy_n(_data.data(), required_size, static_cast<uint8_t *>(mapped_buffer));
        vkUnmapMemory(device, memory);
    }

    { lock_guard<mutex> _(_handle_mutex);
        _handle = buffer;
        _device_memory = memory;
    }

}

TEMPLATE
bool CLASS::is_valid() const {
    return _handle != VK_NULL_HANDLE;
}

TEMPLATE
VkBuffer CLASS::handle() const {
    lock_guard<mutex> _(_handle_mutex);
    return _handle;
}

TEMPLATE
void CLASS::handle_buffer_no_longer_in_use(
    InUseBufferSafeDeletable *deletable
    , VkBuffer buffer
    , VkDeviceMemory memory
) {
    { lock_guard<mutex> _(_available_buffers_mutex);
        auto it = _event_handlers.find(deletable);
        if (it != _event_handlers.end()) {
            _event_handlers.erase(it);
        }

        _available_buffers.emplace_back(make_tuple(buffer, memory));
    }
}

namespace giygas {
    template class WritableBuffer<VK_BUFFER_USAGE_VERTEX_BUFFER_BIT>;
    template class WritableBuffer<VK_BUFFER_USAGE_INDEX_BUFFER_BIT> ;
}
