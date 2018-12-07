#pragma once
#include "VulkanVertexBuffer.hpp"
#include "WritableBuffer.hpp"
#include "ReadOnlyBuffer.hpp"
#include <giygas/EventHandler.hpp>
#include <vulkan/vulkan.h>
#include <memory>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <thread>

namespace giygas {

    class VulkanRenderer;

    template <typename T>
    class VulkanVertexBufferImpl final : public VulkanVertexBuffer {

        T _buffer;

    public:
        explicit VulkanVertexBufferImpl(VulkanRenderer *renderer);
        VulkanVertexBufferImpl(const VulkanVertexBufferImpl &) = delete;
        VulkanVertexBufferImpl &operator=(const VulkanVertexBufferImpl &) = delete;
        VulkanVertexBufferImpl(VulkanVertexBufferImpl &&) noexcept = delete;
        VulkanVertexBufferImpl &operator=(VulkanVertexBufferImpl &&) noexcept = delete;
        ~VulkanVertexBufferImpl() override = default;

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

    extern template class VulkanVertexBufferImpl<ReadOnlyVertexBuffer>;
    extern template class VulkanVertexBufferImpl<WritableVertexBuffer>;

}
