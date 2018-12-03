#pragma once
#include <giygas/IndexBuffer.hpp>
#include <vector>
#include <vulkan/vulkan.h>

namespace giygas {
    using namespace std;

    class VulkanRenderer;

    class VulkanGenericIndexBuffer {
    public:
        virtual ~VulkanGenericIndexBuffer() = default;
        virtual VkBuffer handle() const = 0;
        virtual VkIndexType index_type() const = 0;
    };


    template <typename T, typename DeviceT>
    class VulkanIndexBuffer final : public IndexBuffer<T>, VulkanGenericIndexBuffer {

        VulkanRenderer *_renderer;
        vector<T> _data;
        VkBuffer _buffer;
        VkDeviceMemory _device_memory;

        void safe_delete_resources();

    public:
        explicit VulkanIndexBuffer(VulkanRenderer *renderer);
        VulkanIndexBuffer(const VulkanIndexBuffer &) = delete;
        VulkanIndexBuffer &operator=(const VulkanIndexBuffer &) = delete;
        VulkanIndexBuffer(VulkanIndexBuffer &&) noexcept = delete;
        VulkanIndexBuffer &operator=(VulkanIndexBuffer &&) noexcept = delete;
        ~VulkanIndexBuffer() override;

        //
        // GenericIndexBuffer implementation
        //

        RendererType renderer_type() const override;
        const void *cast_to_specific() const override;

        //
        // IndexBuffer implementation
        //

        void set(size_t index, const T *indices, size_t count) override;
        size_t count() const override;


        //
        // VulkanGenericIndexBuffer implementation
        //

        VkBuffer handle() const override;
        VkIndexType index_type() const override;

    };

}
