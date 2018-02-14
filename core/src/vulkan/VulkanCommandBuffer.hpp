#pragma once
#include <giygas/CommandBuffer.hpp>
#include <vulkan/vulkan.h>
#include "../CommandBufferImpl.hpp"

namespace giygas {

    class VulkanRenderer;
    class VulkanCommandPool;

    class VulkanCommandBuffer final : public CommandBufferImpl {

        VulkanRenderer *_renderer;
        VulkanCommandPool *_pool;
        bool _is_static;
        VkCommandBuffer _handle;

    public:
        VulkanCommandBuffer(VulkanRenderer *renderer, VulkanCommandPool *pool, bool is_static);
        ~VulkanCommandBuffer() override;

        //
        // CommandBufferImpl implementation
        //

        RendererType renderer_type() const override;
        void release() override;
        void record(const DrawInfo &info) override;


        //
        // VulkanCommandBuffer implementation
        //

        void create();
        bool is_static() const;
        VkCommandBuffer handle() const;

    };


}