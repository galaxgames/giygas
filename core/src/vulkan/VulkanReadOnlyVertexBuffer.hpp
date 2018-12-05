#pragma once
#include "VulkanVertexBuffer.hpp"
#include <vulkan/vulkan.h>
#include <memory>

namespace giygas {
    using namespace std;

    class VulkanRenderer;

    class VulkanReadOnlyVertexBuffer final : public VulkanVertexBuffer {

        VulkanRenderer *_renderer = nullptr;
        VkBuffer _handle = VK_NULL_HANDLE;
        VkDeviceMemory _device_memory = VK_NULL_HANDLE;

    public:
        explicit VulkanReadOnlyVertexBuffer(VulkanRenderer *renderer);
        VulkanReadOnlyVertexBuffer(const VulkanReadOnlyVertexBuffer &) = delete;
        VulkanReadOnlyVertexBuffer &operator=(const VulkanReadOnlyVertexBuffer &) = delete;
        VulkanReadOnlyVertexBuffer(VulkanReadOnlyVertexBuffer &&) noexcept = delete;
        VulkanReadOnlyVertexBuffer &operator=(VulkanReadOnlyVertexBuffer &&) noexcept = delete;
        ~VulkanReadOnlyVertexBuffer() override;

        //
        // VertexBuffer implementation
        //

        RendererType renderer_type() const override;

        void set_data(uint32_t offset, const uint8_t *data, uint32_t size) override;
        bool is_valid() const override;
        bool is_writable() const override;

        //
        // VulkanVertexBuffer implementation
        //

        VkBuffer handle() const override;

    };

}
