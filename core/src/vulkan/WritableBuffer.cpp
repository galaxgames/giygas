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
        uint32_t _size = 0;

    public:

        InUseBufferSafeDeletable(InUseBufferSafeDeletable &&) = default;
        InUseBufferSafeDeletable &operator=(InUseBufferSafeDeletable &&) = default;

        InUseBufferSafeDeletable(VkBuffer buffer, VkDeviceMemory memory, uint32_t size) {
            _buffer = buffer;
            _memory = memory;
            _size = size;
        }

        void delete_resources(VulkanRenderer &renderer) override {
            if (_event.has_handlers()) {
                _event.invoke(this);
            } else {
                VkDevice device = renderer.device();
                vkFreeMemory(device, _memory, nullptr);
                vkDestroyBuffer(device, _buffer, nullptr);
            }
        }

        SafeDeletableEventHandler resources_no_longer_in_use() {
            return _event.make_handler();
        }

        VkBuffer buffer() const {
            return _buffer;
        }

        VkDeviceMemory memory() const {
            return _memory;
        }

        uint32_t size() const {
            return _size;
        }

    };

}


TEMPLATE CLASS::WritableBuffer(VulkanRenderer *renderer) {
    _renderer = renderer;
}

TEMPLATE CLASS::~WritableBuffer() {
    _renderer->delete_when_safe(unique_ptr<SwapchainSafeDeleteable>(
        new InUseBufferSafeDeletable(_handle, _device_memory, 0)
    ));
    { lock_guard<mutex> _(_available_buffers_mutex);
        for (tuple<VkBuffer, VkDeviceMemory, uint32_t> buffer_memory_and_size : _available_buffers) {
            _renderer->delete_when_safe(unique_ptr<SwapchainSafeDeleteable>(
                new InUseBufferSafeDeletable(
                    get<0>(buffer_memory_and_size)
                    , get<1>(buffer_memory_and_size)
                    , 0)
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
            auto *in_use_deletable = new InUseBufferSafeDeletable(buffer, memory, _current_buffer_size);
            auto no_longer_in_use_handler = in_use_deletable->resources_no_longer_in_use();
            no_longer_in_use_handler.delegate = BIND_MEMBER1(&WritableBuffer::handle_buffer_no_longer_in_use);
            _event_handlers.emplace(
                piecewise_construct
                , forward_as_tuple(in_use_deletable)
                , forward_as_tuple(move(no_longer_in_use_handler))
            );
            _renderer->delete_when_safe(unique_ptr<SwapchainSafeDeleteable>(in_use_deletable));
            buffer = VK_NULL_HANDLE;
            memory = VK_NULL_HANDLE;
        }

        while (!_available_buffers.empty()) {
            tuple<VkBuffer, VkDeviceMemory, uint32_t> buffer_memory_and_size =  _available_buffers.back();
            _available_buffers.pop_back();
            if (get<2>(buffer_memory_and_size) < required_size) {
                // This buffer isn't big enough. Delete it.
                vkDestroyBuffer(device, get<0>(buffer_memory_and_size), nullptr);
                vkFreeMemory(device, get<1>(buffer_memory_and_size), nullptr);
                continue;
            }
            buffer = get<0>(buffer_memory_and_size);
            memory = get<1>(buffer_memory_and_size);
            _current_buffer_size = get<2>(buffer_memory_and_size);
            break;
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
        _current_buffer_size = required_size;
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
void CLASS::handle_buffer_no_longer_in_use(InUseBufferSafeDeletable *deletable) {
    { lock_guard<mutex> _(_available_buffers_mutex);
        auto it = _event_handlers.find(deletable);
        if (it != _event_handlers.end()) {
            _event_handlers.erase(it);
        }

        _available_buffers.emplace_back(make_tuple(deletable->buffer(), deletable->memory(), deletable->size()));
    }
}

namespace giygas {
    template class WritableBuffer<VK_BUFFER_USAGE_VERTEX_BUFFER_BIT>;
    template class WritableBuffer<VK_BUFFER_USAGE_INDEX_BUFFER_BIT>;
}
