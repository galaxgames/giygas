#pragma once
#include <giygas/VertexBuffer.hpp>
#include <vulkan/vulkan.h>
#include <memory>

namespace giygas {
    using namespace std;

    class VulkanRenderer;

    class VulkanVertexBuffer : public VertexBuffer {

        VulkanRenderer *_renderer;
        VkBuffer _handle;
        VkDeviceMemory _device_memory;
        vector<uint8_t> _data;

        void safe_delete_resources();

    public:
        explicit VulkanVertexBuffer(VulkanRenderer *renderer);
        VulkanVertexBuffer(const VulkanVertexBuffer &) = delete;
        VulkanVertexBuffer &operator=(const VulkanVertexBuffer &) = delete;
        VulkanVertexBuffer(VulkanVertexBuffer &&) noexcept = delete;
        VulkanVertexBuffer &operator=(VulkanVertexBuffer &&) noexcept = delete;
        ~VulkanVertexBuffer() override;

        //
        // VertexBuffer implementation
        //

        RendererType renderer_type() const override;
        void set_data(size_t offset, const uint8_t *data, size_t size) override;
        bool is_valid() const override;

        //
        // VulkanVertexBuffer implementation
        //

        VkBuffer handle() const;

    };

}
