#pragma once
#include <giygas/submission.hpp>
#include <vulkan/vulkan.h>

namespace giygas {

    class VulkanRenderer;
    class VulkanCommandPool;

    class VulkanCommandBuffer final {

        VulkanRenderer *_renderer = nullptr;
        VulkanCommandPool *_pool = nullptr;
        VkCommandBuffer _handle = VK_NULL_HANDLE;

        VkCommandBuffer make_buffer(VkCommandPool pool);
        void free_buffers();
        void record_pass(const PassSubmissionInfo &info, uint32_t swapchain_image_index);
        void record_draw(const DrawInfo &info, VkCommandBuffer handle);

    public:
        VulkanCommandBuffer() = default;
        ~VulkanCommandBuffer();

        //
        // VulkanCommandBuffer implementation
        //
        //RendererType renderer_type() const override;
        void create(VulkanRenderer *renderer, VulkanCommandPool *pool);
        void record(const PassSubmissionInfo *passes, uint32_t pass_count, uint32_t swapchain_image_index);
        void destroy();
        bool is_valid() const;
        VkCommandBuffer handle() const;
    };


}
