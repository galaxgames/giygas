#pragma once
#include <giygas/CommandPool.hpp>
#include <vulkan/vulkan.h>

namespace giygas {

    class VulkanRenderer;
    class VulkanCommandBuffer;

    class VulkanCommandPool final : public CommandPool {

        VulkanRenderer *_renderer;
        VkCommandPool _handle;

    public:

        VulkanCommandPool();

        ~VulkanCommandPool() override;

        //
        // CommandPool implementation
        //

        CommandBuffer *make_buffer() override;
        void reset_buffers() override;


        //
        // VulkanCommandPool implementation
        //

        void create(VulkanRenderer *renderer);
        void destroy();
        VkCommandPool handle() const;

    };

}