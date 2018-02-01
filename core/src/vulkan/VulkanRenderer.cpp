#include <iostream>
#include <cassert>
#include <limits>
#include "VulkanRenderer.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanVertexBuffer.hpp"
#include "VulkanIndexBuffer.hpp"
#include "VulkanMaterial.hpp"
#include "VulkanShader.hpp"

using namespace giygas;
using namespace std;

QueueFamilyIndices::QueueFamilyIndices() {
    graphics_family = static_cast<unsigned int>(-1);
    present_family = static_cast<unsigned int>(-1);
}

bool QueueFamilyIndices::is_complete() const {
    return (
        graphics_family != static_cast<unsigned int>(-1) &&
        present_family != static_cast<unsigned int>(-1)
    );
}

VulkanRenderer::VulkanRenderer(VulkanContext *context) {
    _context = context;
    _instance = nullptr;
    _surface = nullptr;
    _device = nullptr;
    _swapchain = nullptr;
    _image_count = 0;
    _image_view_count = 0;
}

VulkanRenderer::VulkanRenderer(VulkanRenderer &&other) noexcept
    : _images(move(other._images))
    , _image_views(move(other._image_views))
{
    move_common(move(other));
}

VulkanRenderer& VulkanRenderer::operator=(VulkanRenderer &&other) noexcept {
    _images = move(other._images);
    _image_views = move(other._image_views);
    move_common(move(other));
    return *this;
}

void VulkanRenderer::move_common(VulkanRenderer &&other) noexcept {
    _context = other._context;
    _instance = other._instance;
    _surface = other._surface;
    _device = other._device;
    _swapchain = other._swapchain;
    _image_count = other._image_count;
    _image_view_count = other._image_view_count;
    _swapchain_format = other._swapchain_format;
    _swapchain_extent = other._swapchain_extent;
    other._instance = nullptr;
    other._surface = nullptr;
    other._device = nullptr;
    other._swapchain = nullptr;
}

VulkanRenderer::~VulkanRenderer() {
    destroy_image_views(_image_view_count, _image_views.get(), _device);
    vkDestroySwapchainKHR(_device, _swapchain, nullptr);
    vkDestroyDevice(_device, nullptr);
    vkDestroySurfaceKHR(_instance, _surface, nullptr);
    vkDestroyInstance(_instance, nullptr);
}

void VulkanRenderer::initialize() {
    if (!_context->initialize_for_vulkan()) {
        return;
    }

    if (create_instance(_context, _instance) != VK_SUCCESS) {
        return;
    }

    if (_context->create_surface(_instance, &_surface) != VK_SUCCESS) {
        return;
    }

    QueueFamilyIndices queue_family_indices;
    SwapchainInfo swapchain_info;
    VkPhysicalDevice physical_device = find_suitable_physical_device(
        _instance,
        _surface,
        queue_family_indices,
        swapchain_info
    );
    if (physical_device == nullptr) {
        return;
    }

    if (create_logical_device(
        physical_device,
        queue_family_indices,
        _device
    ) != VK_SUCCESS) {
        return;
    }

    _swapchain_format = choose_surface_format(swapchain_info);
    VkPresentModeKHR present_mode = choose_present_mode(swapchain_info);
    _swapchain_extent = choose_swap_extent(swapchain_info);

    if (create_swapchain(
        _device,
        _surface,
        _swapchain_format,
        present_mode,
        _swapchain_extent,
        swapchain_info,
        queue_family_indices,
        _swapchain
    ) != VK_SUCCESS) {
        return;
    }

    _image_count = get_swapchain_images(_device, _swapchain, _images);
    _image_view_count = _image_count;
    if (create_image_views(
        _image_count,
        _images.get(),
        _swapchain_format.format,
        _device,
        _image_views
    ) != VK_SUCCESS) {
        return;
    }
}

VertexBuffer* VulkanRenderer::make_vertex_buffer() {
    return new VulkanVertexBuffer(this);
}


IndexBuffer<uint32_t>* VulkanRenderer::make_index_buffer_32() {
    return new VulkanIndexBuffer<uint32_t>(this);
}

IndexBuffer<uint16_t>* VulkanRenderer::make_index_buffer_16() {
    return new VulkanIndexBuffer<uint16_t>(this);
}

IndexBuffer<uint8_t>* VulkanRenderer::make_index_buffer_8() {
    return new VulkanIndexBuffer<uint8_t>(this);
}

Material* VulkanRenderer::make_material() {
    return new VulkanMaterial(this);
}

Shader* VulkanRenderer::make_shader() {
    return new VulkanShader(this);
}

Texture* VulkanRenderer::make_texture(TextureInitOptions options) {
    return nullptr;
}

FrameBufferSurface *VulkanRenderer::make_framebuffer() {
    return nullptr;
}

RenderBuffer* VulkanRenderer::make_renderbuffer() {
    return nullptr;
}

Pipeline* VulkanRenderer::make_pipeline() {
    return new VulkanPipeline(this);
}

Surface* VulkanRenderer::main_surface() {
    return nullptr;
}

void VulkanRenderer::present() {

}

VkDevice VulkanRenderer::device() const {
    return _device;
}

VkResult VulkanRenderer::create_instance(
    const VulkanContext *context,
    VkInstance &instance
) {
    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "Giygas Application";
    app_info.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
    app_info.pEngineName = "Giygas";
    app_info.engineVersion = VK_MAKE_VERSION(0, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;

    const char **needed_extensions;
    unsigned int needed_extensions_count = 0;
    needed_extensions = context->get_required_instance_extensions(
        &needed_extensions_count
    );

    create_info.enabledExtensionCount = needed_extensions_count;
    create_info.ppEnabledExtensionNames = needed_extensions;

    // Validation layers
    create_info.enabledLayerCount = 0;

    return vkCreateInstance(&create_info, nullptr, &instance);
}

bool VulkanRenderer::is_physical_device_suitable(
    VkPhysicalDevice device,
    VkSurfaceKHR surface,
    QueueFamilyIndices &queue_family_indices,
    SwapchainInfo &swapchain_info
) {
    queue_family_indices = find_queue_family_indices(device, surface);
    if (!queue_family_indices.is_complete()) {
        return false;
    }
    if (!physical_device_has_required_extensions(device)) {
        return false;
    }
    get_swap_chain_info(device, surface, swapchain_info);
    return swapchain_is_adequate(swapchain_info);
}

bool VulkanRenderer::physical_device_has_required_extensions(
    VkPhysicalDevice device
) {
    vector<const char *> required_extensions = get_required_device_extensions();

    unordered_set<string> unfound_extensions(
        required_extensions.begin(),
        required_extensions.end()
    );

    unsigned int available_extension_count;
    vkEnumerateDeviceExtensionProperties(
        device,
        nullptr,
        &available_extension_count,
        nullptr
    );

    unique_ptr<VkExtensionProperties[]> available_extensions(
        new VkExtensionProperties[available_extension_count]
    );
    vkEnumerateDeviceExtensionProperties(
        device,
        nullptr,
        &available_extension_count,
        available_extensions.get()
    );

    for (unsigned int i = 0; i < available_extension_count; ++i) {
        const VkExtensionProperties &extension = available_extensions[i];
        unfound_extensions.erase(extension.extensionName);
    }

    return unfound_extensions.empty();
}

bool VulkanRenderer::swapchain_is_adequate(const SwapchainInfo &info) {
    return info.format_count > 0 && info.present_mode_count > 0;
}

QueueFamilyIndices VulkanRenderer::find_queue_family_indices(
    VkPhysicalDevice device,
    VkSurfaceKHR surface
) {
    unsigned int queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(
        device,
        &queue_family_count,
        nullptr
    );

    unique_ptr<VkQueueFamilyProperties[]> queue_families(
        new VkQueueFamilyProperties[queue_family_count]
    );
    vkGetPhysicalDeviceQueueFamilyProperties(
        device,
        &queue_family_count,
        queue_families.get()
    );

    QueueFamilyIndices indices;
    for (unsigned int i = 0; i < queue_family_count; ++i) {
        const VkQueueFamilyProperties &family = queue_families[i];
        if (family.queueCount > 0) {
            if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphics_family = i;
            }

            VkBool32 surface_supported;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &surface_supported);
            if (surface_supported == VK_TRUE) {
                indices.present_family = i;
            }
        }
    }
    return indices;
}

VkPhysicalDevice VulkanRenderer::find_suitable_physical_device(
    VkInstance instance,
    VkSurfaceKHR surface,
    QueueFamilyIndices &queue_family_indices,
    SwapchainInfo &swapchain_info
) {
    unsigned int device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

    unique_ptr<VkPhysicalDevice[]> devices(new VkPhysicalDevice[device_count]);
    vkEnumeratePhysicalDevices(instance, &device_count, devices.get());

    for (int i = 0; i < device_count; ++i) {
        VkPhysicalDevice device = devices[i];
        if (is_physical_device_suitable(
            device,
            surface,
            queue_family_indices,
            swapchain_info
        )) {
            return device;
        }
    }

    return VK_NULL_HANDLE;
}

VkResult VulkanRenderer::create_logical_device(
    VkPhysicalDevice physical_device,
    QueueFamilyIndices queue_family_indices,
    VkDevice &logical_device
) {
    float queue_priority = 1.0f;
    VkDeviceQueueCreateInfo queue_create_info = {};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queue_family_indices.graphics_family;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queue_priority;

    vector<const char *> extensions = get_required_device_extensions();

    VkDeviceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.queueCreateInfoCount = 1;
    create_info.pQueueCreateInfos = &queue_create_info;
    create_info.enabledExtensionCount
        = static_cast<unsigned int>(extensions.size());
    create_info.ppEnabledExtensionNames = extensions.data();
    create_info.enabledLayerCount = 0;

    return vkCreateDevice(
        physical_device,
        &create_info,
        nullptr,
        &logical_device
    );
}

vector<const char *> VulkanRenderer::get_required_device_extensions() {
    return vector<const char *> {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
}

void VulkanRenderer::get_swap_chain_info(
    VkPhysicalDevice device,
    VkSurfaceKHR surface,
    SwapchainInfo &info
) {
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &info.capabilities);

    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &info.format_count, nullptr);
    info.formats = unique_ptr<VkSurfaceFormatKHR[]>(new VkSurfaceFormatKHR[info.format_count]);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &info.format_count, info.formats.get());

    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &info.present_mode_count, nullptr);
    info.present_modes = unique_ptr<VkPresentModeKHR[]>(new VkPresentModeKHR[info.present_mode_count]);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &info.present_mode_count, info.present_modes.get());
}

VkSurfaceFormatKHR VulkanRenderer::choose_surface_format(
    const SwapchainInfo &info
) {
    assert(info.format_count > 0);

    if (
        info.format_count == 1 &&
        info.formats[0].format == VK_FORMAT_UNDEFINED
    ) {
        VkSurfaceFormatKHR format;
        format.format = VK_FORMAT_B8G8R8A8_UNORM;
        format.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        return format;
    }

    // TODO: Make this smarter!
    for (size_t i = 0; i < info.format_count; ++i) {
        const VkSurfaceFormatKHR &format = info.formats[i];
        if (
            format.format == VK_FORMAT_B8G8R8A8_UNORM &&
            format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
        ) {
            return format;
        }
    }

    return info.formats[0];
}

VkPresentModeKHR VulkanRenderer::choose_present_mode(
    const SwapchainInfo &info
) {
    // VK_PRESENT_MODE_FIFO_KHR is guaranteed to always be available.
    VkPresentModeKHR selected_mode = VK_PRESENT_MODE_FIFO_KHR;

    // TODO: Make this smarter and capable of choosing different modes.
    for (unsigned int i = 0; i < info.present_mode_count; ++i) {
        VkPresentModeKHR mode = info.present_modes[i];
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return mode;
        }
    }

    return selected_mode;
}

VkExtent2D VulkanRenderer::choose_swap_extent(const SwapchainInfo &info) {
    if (info.capabilities.currentExtent.width != numeric_limits<uint32_t>::max()) {
        return info.capabilities.currentExtent;
    }

    // TODO: Get real value from giygas context
    VkExtent2D extent = {800, 800};

    extent.width = std::max(
        info.capabilities.minImageExtent.width,
        std::min(
            info.capabilities.maxImageExtent.width,
            extent.width
        )
    );
    extent.height = std::max(
        info.capabilities.minImageExtent.height,
        std::min(
            info.capabilities.maxImageExtent.height,
            extent.height
        )
    );

    return extent;
}

VkResult VulkanRenderer::create_swapchain(
    VkDevice device,
    VkSurfaceKHR surface,
    VkSurfaceFormatKHR surface_format,
    VkPresentModeKHR present_mode,
    VkExtent2D extent,
    const SwapchainInfo &info,
    QueueFamilyIndices indices,
    VkSwapchainKHR &swapchain
) {
    unsigned int image_count = info.capabilities.minImageCount + 1;
    if (
        info.capabilities.maxImageCount > 0 &&
        image_count > info.capabilities.maxImageCount
    ) {
        image_count = info.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = surface;
    create_info.minImageCount = image_count;
    create_info.imageFormat = surface_format.format;
    create_info.imageColorSpace = surface_format.colorSpace;
    create_info.imageExtent = extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (indices.graphics_family != indices.present_family) {
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = reinterpret_cast<unsigned int *>(&indices);
    }
    else {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        create_info.queueFamilyIndexCount = 0;
        create_info.pQueueFamilyIndices = nullptr;
    }

    create_info.preTransform = info.capabilities.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = present_mode;
    create_info.clipped = VK_TRUE;
    create_info.oldSwapchain = VK_NULL_HANDLE;

    return vkCreateSwapchainKHR(device, &create_info, nullptr, &swapchain);
}

unsigned int VulkanRenderer::get_swapchain_images(
    VkDevice device,
    VkSwapchainKHR swapchain,
    unique_ptr<VkImage[]> &dest
) {
    unsigned int count;
    vkGetSwapchainImagesKHR(device, swapchain, &count, nullptr);
    dest = unique_ptr<VkImage[]>(new VkImage[count]);
    vkGetSwapchainImagesKHR(device, swapchain, &count, dest.get());
    return count;
}

VkResult VulkanRenderer::create_image_views(
    unsigned int count,
    const VkImage *images,
    VkFormat format,
    VkDevice device,
    unique_ptr<VkImageView[]> &views
) {
    views = unique_ptr<VkImageView[]>(new VkImageView[count]);
    VkImageViewCreateInfo create_info;
    VkResult create_result;

    create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    create_info.format = format;
    create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    create_info.subresourceRange.baseMipLevel = 0;
    create_info.subresourceRange.levelCount = 1;
    create_info.subresourceRange.baseArrayLayer = 0;
    create_info.subresourceRange.layerCount = 1;

    for (unsigned int i = 0; i < count; ++i){
        views[i] = nullptr;
    }
    for (unsigned int i = 0; i < count; ++i) {
        create_info.image = images[i];

        create_result = vkCreateImageView(
            device,
            &create_info,
            nullptr,
            &views[i]
        );
        if (create_result != VK_SUCCESS) {
            return create_result;
        }
    }
    return VK_SUCCESS;
}

void VulkanRenderer::destroy_image_views(
    unsigned int count,
    VkImageView *views,
    VkDevice device
) {
    for (unsigned int i = 0; i < count; ++i) {
        vkDestroyImageView(device, views[i], nullptr);
    }
}
