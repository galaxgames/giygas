#pragma once
#include <giygas/Renderer.hpp>
#include <giygas/Context.hpp>
#include <vulkan/vulkan.h>
#include <giygas/VulkanContext.hpp>


namespace giygas {
    using namespace std;

    class QueueFamilyIndices {
    public:
        unsigned int graphics_family;
        unsigned int present_family;

        QueueFamilyIndices();
        bool is_complete() const;
    };

    class SwapchainInfo {
    public:
        VkSurfaceCapabilitiesKHR capabilities;
        unsigned int format_count;
        unsigned int present_mode_count;
        unique_ptr<VkSurfaceFormatKHR[]> formats;
        unique_ptr<VkPresentModeKHR[]> present_modes;
    };


    class VulkanRenderer : public Renderer {
        VulkanContext *_context;
        VkInstance _instance;
        VkSurfaceKHR _surface;
        VkDevice _device;
        VkSwapchainKHR _swapchain;
        unsigned int _image_count;
        unique_ptr<VkImage[]> _images;
        VkSurfaceFormatKHR _swapchain_format;
        VkExtent2D _swapchain_extent;

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

        static unsigned int get_swapchain_images(
            VkDevice device,
            VkSwapchainKHR swapchain,
            unique_ptr<VkImage[]> &dest
        );

    public:
        explicit VulkanRenderer(VulkanContext *context);
        VulkanRenderer(const VulkanRenderer &) = delete;
        VulkanRenderer &operator=(const VulkanRenderer &) = delete;
        VulkanRenderer(VulkanRenderer &&) noexcept;
        VulkanRenderer &operator=(VulkanRenderer &&) noexcept;
        ~VulkanRenderer() override;

        void initialize() override;

        VertexBuffer *make_vbo() override;
        VertexArray *make_vao() override;
        ElementBuffer<unsigned int> *make_int_ebo() override;
        ElementBuffer<unsigned short> *make_short_ebo() override;
        ElementBuffer<unsigned char> *make_char_ebo() override;
        Material *make_material() override;
        Shader *make_shader() override;
        Texture *make_texture(TextureInitOptions options) override;
        FrameBufferSurface *make_framebuffer() override;
        RenderBuffer *make_renderbuffer() override;
        Surface *main_surface() override;

        void present() override;
    };

}