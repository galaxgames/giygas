#pragma once
#include <giygas/Renderer.hpp>
#include <giygas/Context.hpp>
#include <vulkan/vulkan.h>
#include <giygas/VulkanContext.hpp>
#include "VulkanCommandPool.hpp"


namespace giygas {
    using namespace std;

    class QueueFamilyIndices {
    public:
        uint32_t graphics_family;
        uint32_t present_family;

        QueueFamilyIndices();
        bool is_complete() const;
    };

    class SwapchainInfo {
    public:
        VkSurfaceCapabilitiesKHR capabilities;
        uint32_t format_count;
        uint32_t present_mode_count;
        unique_ptr<VkSurfaceFormatKHR[]> formats;
        unique_ptr<VkPresentModeKHR[]> present_modes;
    };


    class VulkanRenderer final : public Renderer {
        VulkanContext *_context;
        VkInstance _instance;
        VkSurfaceKHR _surface;
        VkDevice _device;
        VkSwapchainKHR _swapchain;
        QueueFamilyIndices _queue_family_indices;
        VkQueue _graphics_queue;
        VkQueue _present_queue;
        uint32_t _image_count;
        uint32_t _image_view_count;
        unique_ptr<VkImage[]> _images;
        unique_ptr<VkImageView[]> _image_views;
        VkSurfaceFormatKHR _swapchain_format;
        VkExtent2D _swapchain_extent;
        VkPhysicalDeviceMemoryProperties _memory_properties;
        VulkanCommandPool _copy_command_pool;
        VkSemaphore image_available_semaphore;
        VkSemaphore render_finished_semaphore;

        void move_common(VulkanRenderer &&other) noexcept;

        static VkResult create_instance(
            const VulkanContext *context,
            VkInstance &instance
        );

        static bool is_physical_device_suitable(
            VkPhysicalDevice device,
            VkSurfaceKHR surface,
            QueueFamilyIndices &queue_family_indices,
            SwapchainInfo &swapchain_info
        );

        static bool physical_device_has_required_extensions(VkPhysicalDevice device);
        static bool swapchain_is_adequate(const SwapchainInfo& info);

        static QueueFamilyIndices find_queue_family_indices(
            VkPhysicalDevice device,
            VkSurfaceKHR surface
        );

        static VkPhysicalDevice find_suitable_physical_device(
            VkInstance instance,
            VkSurfaceKHR surface,
            QueueFamilyIndices &queue_family_indices,
            SwapchainInfo &swapchain_info
        );

        static VkResult create_logical_device(
            VkPhysicalDevice physical_device,
            QueueFamilyIndices queue_family_indices,
            VkDevice &logical_device
        );

        static vector<const char *> get_required_device_extensions();

        static void get_swap_chain_info(
            VkPhysicalDevice device,
            VkSurfaceKHR surface,
            SwapchainInfo &info
        );

        static VkSurfaceFormatKHR choose_surface_format(const SwapchainInfo &info);
        static VkPresentModeKHR choose_present_mode(const SwapchainInfo &info);
        static VkExtent2D choose_swap_extent(const SwapchainInfo &info);

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

        static void destroy_image_views(
            uint32_t count,
            VkImageView *views,
            VkDevice device
        );

    public:
        explicit VulkanRenderer(VulkanContext *context);
        VulkanRenderer(const VulkanRenderer &) = delete;
        VulkanRenderer &operator=(const VulkanRenderer &) = delete;
        VulkanRenderer(VulkanRenderer &&) noexcept;
        VulkanRenderer &operator=(VulkanRenderer &&) noexcept;
        ~VulkanRenderer() override;

        //
        // Renderer implementation
        //

        void initialize() override;

        VertexBuffer *make_vertex_buffer() override;
        IndexBuffer<uint32_t> *make_index_buffer_32() override;
        IndexBuffer<uint16_t> *make_index_buffer_16() override;
        IndexBuffer<uint8_t> *make_index_buffer_8() override;
        Material *make_material() override;
        Shader *make_shader() override;
        Texture *make_texture(SamplerOptions options) override;
        Framebuffer *make_framebuffer() override;
        RenderBuffer *make_renderbuffer() override;
        Pipeline *make_pipeline() override;
        Surface *main_surface() override;

        void present() override;


        //
        // VulkanRenderer implementation
        //

        VkDevice device() const;
        const QueueFamilyIndices &queue_family_indices() const;
        VkCommandPool copy_command_pool() const;
        VkQueue graphics_queue() const;

        bool find_memory_type(
            uint32_t type_filter,
            VkMemoryPropertyFlags properties,
            uint32_t& found_memory_type
        ) const;

        void create_buffer(
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VkMemoryPropertyFlags memory_properties,
            VkBuffer &buffer,
            VkDeviceMemory &device_memory
        ) const;

        VkResult copy_buffer(
            VkBuffer src,
            VkBuffer dest,
            VkDeviceSize size
        ) const;

    };

}