#pragma once
#include <giygas/CommandPool.hpp>
#include <vulkan/vulkan.h>

namespace giygas {

    class VulkanRenderer;

    class VulkanCommandPool final : public CommandPool {

        VulkanRenderer *_renderer;
        VkCommandPool _handle;

    public:

        VulkanCommandPool();

        ~VulkanCommandPool() override;

        //
        // CommandPool implementation
        //

        void draw(const DrawInfo &info) override;

        //
        // VulkanCommandPool implementation
        //

        void create(VulkanRenderer *renderer);
        VkCommandPool handle() const;

    };

}