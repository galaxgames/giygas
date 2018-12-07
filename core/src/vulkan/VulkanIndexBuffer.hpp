#pragma once
#include <giygas/IndexBuffer.hpp>
#include <vector>
#include <vulkan/vulkan.h>
#include "ReadOnlyBuffer.hpp"
#include "WritableBuffer.hpp"

namespace giygas {
    using namespace std;

    class VulkanRenderer;

    class VulkanGenericIndexBuffer {
    public:
        virtual ~VulkanGenericIndexBuffer() = default;
        virtual VkBuffer handle() const = 0;
        virtual VkIndexType index_type() const = 0;
    };


    template <typename T, typename DeviceT, typename BufferT>
    class VulkanIndexBuffer final : public IndexBuffer<T>, VulkanGenericIndexBuffer {

        BufferT _buffer;
        uint32_t _count = 0;

    public:
        explicit VulkanIndexBuffer(VulkanRenderer *renderer);
        VulkanIndexBuffer(const VulkanIndexBuffer &) = delete;
        VulkanIndexBuffer &operator=(const VulkanIndexBuffer &) = delete;
        VulkanIndexBuffer(VulkanIndexBuffer &&) noexcept = delete;
        VulkanIndexBuffer &operator=(VulkanIndexBuffer &&) noexcept = delete;
        ~VulkanIndexBuffer() override = default;

        //
        // GenericIndexBuffer implementation
        //

        RendererType renderer_type() const override;
        const void *cast_to_specific() const override;

        //
        // IndexBuffer implementation
        //

        void set(uint32_t index, const T *indices, uint32_t count) override;
        size_t count() const override;


        //
        // VulkanGenericIndexBuffer implementation
        //

        VkBuffer handle() const override;
        VkIndexType index_type() const override;

    };

}
