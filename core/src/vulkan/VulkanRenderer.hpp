#pragma once
#include <giygas/Renderer.hpp>
#include <giygas/Context.hpp>
#include <vulkan/vulkan.h>
#include <giygas/VulkanContext.hpp>
#include "VulkanCommandPool.hpp"
#include "QueueFamilyIndices.hpp"
#include "SwapchainInfo.hpp"
#include "VulkanSwapchain.hpp"
#include <limits>
#include <queue>


namespace giygas {

    class VulkanRenderer final : public Renderer {

        VulkanContext *_context = nullptr;
        VkInstance _instance = nullptr;
        VkSurfaceKHR _surface = nullptr;
        VkPhysicalDevice _physical_device = nullptr;
        VkDevice _device = nullptr;
        QueueFamilyIndices _queue_family_indices;
        VkQueue _graphics_queue = nullptr;
        VkQueue _present_queue = nullptr;
        VulkanSwapchain _swapchain;
        VkPhysicalDeviceMemoryProperties _memory_properties = {};
        VulkanCommandPool _copy_command_pool;
        unique_ptr<VkSemaphore[]> _swapchain_image_available_semaphores;
        //VkFence _command_buffers_executed_fence = nullptr;
        VkDescriptorPool _shared_descriptor_pool = nullptr;

        std::queue<VkFence> _command_buffers_executed_fences;

        uint32_t _current_image_acquisition_semaphore_index = 0;
        uint32_t _next_swapchain_image_index = 0;
        uint32_t _previous_swapchain_image_index = std::numeric_limits<uint32_t>::max();

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

    public:
        explicit VulkanRenderer(VulkanContext *context);
        VulkanRenderer(const VulkanRenderer &) = delete;
        VulkanRenderer &operator=(const VulkanRenderer &) = delete;
        VulkanRenderer(VulkanRenderer &&) noexcept = delete;
        VulkanRenderer &operator=(VulkanRenderer &&) noexcept = delete;
        ~VulkanRenderer() override;

        //
        // Renderer implementation
        //

        void initialize() override;
        RendererType renderer_type() const override;
        VertexBuffer *make_vertex_buffer() override;
        IndexBuffer<uint32_t> *make_index_buffer_32() override;
        IndexBuffer<uint16_t> *make_index_buffer_16() override;
        IndexBuffer<uint8_t> *make_index_buffer_8() override;
        UniformBuffer *make_uniform_buffer() override;
        Shader *make_shader() override;
        Texture *make_texture() override;
        Sampler *make_sampler() override;
        DescriptorPool *make_descriptor_pool() override;
        DescriptorSet *make_descriptor_set() override;
        RenderPass *make_render_pass() override;
        Framebuffer *make_framebuffer() override;
        Pipeline *make_pipeline() override;
        CommandPool *make_command_pool() override;
        CommandBuffer *make_command_buffer() override;

        const  RenderTarget *swapchain() const override;

        uint32_t get_api_texture_format(TextureFormat format) const override;

        void submit(const CommandBuffer **buffers, size_t buffer_count) override;

        //
        // VulkanRenderer implementation
        //

        VkPhysicalDevice physical_device() const;
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

        uint32_t next_swapchain_image_index() const;
        uint32_t swapchain_image_count() const;

        static VkFormat translate_texture_format(TextureFormat format);

    };

}
