#pragma once
#include <giygas/UniformBuffer.hpp>
#include <vulkan/vulkan.h>
#include <vector>

namespace giygas {
    using namespace std;

    class VulkanRenderer;

    class VulkanUniformBuffer final : public UniformBuffer {

        VulkanRenderer *_renderer;
        VkBuffer _handle;
        VkDeviceMemory _device_memory;
        vector<uint8_t> _data;
        void *_mapped_buffer;

    public:
        explicit VulkanUniformBuffer(VulkanRenderer *renderer);
        ~VulkanUniformBuffer() override;

        //
        // UniformBuffer implementation
        //

        RendererType renderer_type() const override;
        void set_data(uint32_t offset, const uint8_t *data, uint32_t size) override;


        //
        // VulkanUniformBuffer implementation
        //

        VkBuffer handle() const;
        size_t size() const;
    };

}
