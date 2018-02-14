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

        //void draw(const DrawInfo &info) override;
        CommandBuffer take_static_buffer() override;
        CommandBuffer take_dynamic_buffer() override;


        //
        // VulkanCommandPool implementation
        //

        void create(VulkanRenderer *renderer);
        void return_buffer(VulkanCommandBuffer *buffer);
        VkCommandPool handle() const;

    };

}