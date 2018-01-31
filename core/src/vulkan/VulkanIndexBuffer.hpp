#pragma once
#include <giygas/IndexBuffer.hpp>
#include <vector>
#include <vulkan/vulkan.h>

namespace giygas {
    using namespace std;

    class VulkanRenderer;

    template <typename T>
    class VulkanIndexBuffer : IndexBuffer<T> {

        VulkanRenderer *_renderer;
        vector<T> _data;
        VkBuffer _buffer;
        VkDeviceMemory _device_memory;

    public:
        explicit VulkanIndexBuffer(VulkanRenderer *renderer);
        VulkanIndexBuffer(const VulkanIndexBuffer &) = delete;
        VulkanIndexBuffer &operator=(const VulkanIndexBuffer &) = delete;
        VulkanIndexBuffer(VulkanIndexBuffer &&) noexcept = delete;
        VulkanIndexBuffer &operator=(VulkanIndexBuffer &&) noexcept = delete;
        ~VulkanIndexBuffer() override;

        //
        // IndexBuffer implementation
        //

        RendererType get_renderer_type() const override;
        void set(size_t index, const T *indices, size_t count) override;
        size_t count() const override;


        //
        // VulkanIndexBuffer implementation
        //



    };

    extern template class VulkanIndexBuffer<uint32_t>;
    extern template class VulkanIndexBuffer<uint16_t>;
    extern template class VulkanIndexBuffer<uint8_t>;
}