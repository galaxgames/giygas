#pragma once
#include <giygas/CommandBuffer.hpp>
#include <vulkan/vulkan.h>

namespace giygas {

    class VulkanRenderer;
    class VulkanCommandPool;

    class VulkanCommandBuffer final : public CommandBuffer {

        VulkanRenderer *_renderer;
        VulkanCommandPool *_pool;
        VkCommandBuffer _handle;

    public:
        VulkanCommandBuffer(VulkanRenderer *renderer, VulkanCommandPool *pool /*, bool is_static */);
        ~VulkanCommandBuffer() override;

        //
        // CommandBufferImpl implementation
        //

        RendererType renderer_type() const override;
        void record(const DrawInfo &info) override;


        //
        // VulkanCommandBuffer implementation
        //

        void create();
        VkCommandBuffer handle() const;

    };


}