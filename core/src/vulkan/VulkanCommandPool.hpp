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
        explicit VulkanCommandPool(VulkanRenderer *renderer);
        ~VulkanCommandPool() override;

        //
        // CommandPool implementation
        //
        RendererType renderer_type() const override;
        void create() override;
        void reset_buffers() override;
        bool is_valid() const override;

        //
        // VulkanCommandPool implementation
        //

        void destroy();
        VkCommandPool handle() const;

    };

}