#pragma once
#include <vulkan/vulkan.h>
#include <memory>

namespace giygas {

    class VulkanRenderer;
    class VulkanCommandBuffer;

    class VulkanCommandPool final {

        VulkanRenderer *_renderer = nullptr;
        VkCommandPool _handle = VK_NULL_HANDLE;

        VkCommandPool make_pool() const;

    public:
        VulkanCommandPool() = default;
        ~VulkanCommandPool();

        //
        // VulkanCommandPool implementation
        //
        //RendererType renderer_type() const override;
        void create(VulkanRenderer *renderer);
        void reset_buffers();
        bool is_valid() const;
        void destroy();
        VkCommandPool handle() const;
    };

}
