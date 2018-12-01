#pragma once
#include <giygas/CommandBuffer.hpp>
#include <vulkan/vulkan.h>

namespace giygas {

    class VulkanRenderer;
    class VulkanCommandPool;

    class VulkanCommandBuffer final : public CommandBuffer {

        VulkanRenderer *_renderer = nullptr;
        VulkanCommandPool *_pool = nullptr;
        unique_ptr<VkCommandBuffer[]> _handles;

        VkCommandBuffer make_buffer(VkCommandPool pool) const;
        void free_buffers();
        void record_draw(const DrawInfo &info, VkCommandBuffer handle) const;

    public:
        VulkanCommandBuffer(VulkanRenderer *renderer);
        ~VulkanCommandBuffer() override;

        //
        // CommandBufferImpl implementation
        //

        RendererType renderer_type() const override;
        void record_pass(const SingleBufferPassInfo &info) override;
        void create(CommandPool *pool) override;
        bool is_valid() const override;

        //
        // VulkanCommandBuffer implementation
        //
        VkCommandBuffer get_handle(uint32_t index) const;
    };


}
