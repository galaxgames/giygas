#pragma once
#include <giygas/RendererType.hpp>
#include <vulkan/vulkan.h>
#include <memory>
#include "SwapchainInfo.hpp"
#include "QueueFamilyIndices.hpp"
#include "VulkanFramebuffer.hpp"
#include "VulkanSwapchainRenderTarget.hpp"

namespace giygas {
    using namespace std;

    class VulkanRenderer;

    class VulkanSwapchain {

        VulkanRenderer *_renderer;
        VkSwapchainKHR _swapchain;
        uint32_t _image_count;
        uint32_t _image_view_count;
        unique_ptr<VkImage[]> _images;
        unique_ptr<VkImageView[]> _image_views;
        unique_ptr<VulkanSwapchainRenderTarget[]> _render_targets;
        VkSurfaceFormatKHR _format;
        VkExtent2D _extent;

        static VkResult create_swapchain(
            VkDevice device,
            VkSurfaceKHR surface,
            VkSurfaceFormatKHR surface_format,
            VkPresentModeKHR present_mode,
            VkExtent2D extent,
            const SwapchainInfo &info,
            QueueFamilyIndices indices,
            VkSwapchainKHR &swapchain
        );

        static uint32_t get_swapchain_images(
            VkDevice device,
            VkSwapchainKHR swapchain,
            unique_ptr<VkImage[]> &dest
        );

        static VkResult create_image_views(
            uint32_t count,
            const VkImage *images,
            VkFormat format,
            VkDevice device,
            unique_ptr<VkImageView[]> &views
        );

        //static void destroy_image_views(uint32_t count, VkImageView *views, VkDevice device);

    public:
        explicit VulkanSwapchain();
        VulkanSwapchain(const VulkanSwapchain &) = delete;
        VulkanSwapchain &operator=(const VulkanSwapchain &) = delete;
        VulkanSwapchain(VulkanSwapchain &&) = delete;
        VulkanSwapchain &operator=(VulkanSwapchain &&) = delete;
        ~VulkanSwapchain();

        uint32_t width() const;
        uint32_t height() const;

        bool create(
            VulkanRenderer *renderer,
            VkSurfaceKHR surface,
            VkSurfaceFormatKHR surface_format,
            VkPresentModeKHR present_mode,
            VkExtent2D extent,
            const SwapchainInfo &info
        );

        void destroy();

        VkSwapchainKHR handle() const;
        uint32_t image_count() const;
        const VkSurfaceFormatKHR &surface_format() const;
        VkImageView get_image_view(uint32_t index) const;
        const VulkanSwapchainRenderTarget *get_render_target(uint32_t index) const;


    };

}