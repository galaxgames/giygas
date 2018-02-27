#pragma once
#include <giygas/CommandBuffer.hpp>
#include <vulkan/vulkan.h>

namespace giygas {

    class VulkanRenderer;
    class VulkanCommandPool;

    class VulkanCommandBuffer final : public CommandBuffer {

        VulkanRenderer *_renderer;
        const VulkanCommandPool *_pool;
        VkCommandBuffer _handle;

        void record_draw(const DrawInfo &info) const;

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

        VkCommandBuffer handle() const;

    };


}