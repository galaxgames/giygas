#include <cassert>
#include <limits>
#include "VulkanRenderer.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanVertexBuffer.hpp"
#include "VulkanIndexBuffer.hpp"
#include "VulkanShader.hpp"
#include "VulkanTexture.hpp"
#include "VulkanFramebuffer.hpp"
#include "VulkanCommandBuffer.hpp"
#include "VulkanUniformBuffer.hpp"
#include "VulkanSampler.hpp"
#include "VulkanDescriptorPool.hpp"
#include "VulkanDescriptorSet.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanVertexBufferImpl.cpp"
#include <giygas/validation/submission_validation.hpp>

using namespace giygas;
using namespace std;

VulkanRenderer::VulkanRenderer(VulkanContext *context) {
    _context = context;
}

VulkanRenderer::~VulkanRenderer() {
    // call the destroy method here, as it will add safe deletables to the queue, which we process below.
    finish_enqueued_command_buffers();

    for (uint32_t i = 0, ilen = _swapchain.image_count(); i < ilen; ++i) {
        vkDestroyFence(_device, _fences_by_submission[i], nullptr);
        vkDestroySemaphore(_device, _swapchain_image_available_semaphores[i], nullptr);
        delete_resources_for_image_index(i);
        _command_buffers_by_submission[i].destroy();
        _command_pools_by_submission[i].destroy();
    }

    _copy_command_pool.destroy();
    _swapchain.destroy();

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

    SwapchainInfo swapchain_info;
    VkPhysicalDevice physical_device = find_suitable_physical_device(
        _instance,
        _surface,
        _queue_family_indices,
        swapchain_info
    );
    _physical_device = physical_device;
    if (physical_device == nullptr) {
        return;
    }

    if (create_logical_device(
        physical_device,
        _queue_family_indices,
        _device
    ) != VK_SUCCESS) {
        return;
    }

    VkSurfaceFormatKHR swapchain_format = choose_surface_format(swapchain_info);
    VkPresentModeKHR present_mode = choose_present_mode(swapchain_info);
    VkExtent2D swapchain_extent = choose_swap_extent(swapchain_info);

    _swapchain.create(
        this,
        _surface,
        swapchain_format,
        present_mode,
        swapchain_extent,
        swapchain_info
    );

    vkGetPhysicalDeviceMemoryProperties(physical_device, &_memory_properties);
    vkGetDeviceQueue(_device, _queue_family_indices.graphics_family, 0, &_graphics_queue);
    vkGetDeviceQueue(_device, _queue_family_indices.present_family, 0, &_present_queue);

    uint32_t image_count = _swapchain.image_count();
    _swapchain_image_available_semaphores = unique_ptr<VkSemaphore[]>(new VkSemaphore[image_count]);
    _fences_by_submission = unique_ptr<VkFence[]>(new VkFence[image_count]);
    _command_pools_by_submission = unique_ptr<VulkanCommandPool[]>(new VulkanCommandPool[image_count]);
    _command_buffers_by_submission = unique_ptr<VulkanCommandBuffer[]>(new VulkanCommandBuffer[image_count]);
    _command_buffer_handles_by_submission = unique_ptr<VkCommandBuffer[]>(new VkCommandBuffer[image_count]);
    _image_indices_by_submission = unique_ptr<uint32_t[]>(new uint32_t[image_count]);

    //
    // Create semaphores and other per submission resources
    //
    VkSemaphoreCreateInfo semaphore_info = {};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    for (uint32_t i = 0; i < image_count; ++i) {
        vkCreateSemaphore(_device, &semaphore_info, nullptr, &_swapchain_image_available_semaphores[i]);
        _command_pools_by_submission[i].create(this);
        _command_buffers_by_submission[i].create(this, &_command_pools_by_submission[i]);
        _command_buffer_handles_by_submission[i] = _command_buffers_by_submission[i].handle();
        _image_indices_by_submission[i] = numeric_limits<uint32_t>::max();
    }

    //
    // Create command buffer execution fences
    //
    VkFence fence;
    VkFenceCreateInfo fence_info = {};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    // submit the first fences as signalled, as we don't actually have any command buffers to wait on, given that we
    // just started the engine.
    for (uint32_t i = 1; i < image_count; ++i) {
        vkCreateFence(_device, &fence_info, nullptr, &fence);
        _fences_by_submission[i] = fence;
    }

    // Submit the last fence as unsigned, as this is the first fence that will be given to vkQueueSubmit.
    fence_info.flags = 0;
    vkCreateFence(_device, &fence_info, nullptr, &fence);
    _fences_by_submission[0] = fence;

    // Fill up the submission indices
    for (uint32_t i = image_count; i > 0; i--) {
        _submissions.push(i - 1);
    }

    _copy_command_pool.create(this);

    // Create swapchain safe deletable lists for each submission
    _safe_deletables_by_image_index = unique_ptr<vector<unique_ptr<SwapchainSafeDeleteable>>[]>(
        new vector<unique_ptr<SwapchainSafeDeleteable>> [_swapchain.image_count()]
    );

}

RendererType VulkanRenderer::renderer_type() const {
    return RendererType::Vulkan;
}

VertexBuffer* VulkanRenderer::make_vertex_buffer(VertexBufferCreateFlags flags) {
    if (flags & VertexBufferCreateFlag_Writable) {
        return new VulkanVertexBufferImpl<WritableVertexBuffer>(this);
    } else {
        return new VulkanVertexBufferImpl<ReadOnlyVertexBuffer>(this);
    }
}

IndexBuffer<uint32_t>* VulkanRenderer::make_index_buffer_32(IndexBufferCreateFlags flags) {
    if (flags & VertexBufferCreateFlag_Writable) {
        return new VulkanIndexBuffer<uint32_t, uint32_t, WritableIndexBuffer>(this);
    } else {
        return new VulkanIndexBuffer<uint32_t, uint32_t, ReadOnlyIndexBuffer>(this);
    }
}

IndexBuffer<uint16_t>* VulkanRenderer::make_index_buffer_16(IndexBufferCreateFlags flags) {
    if (flags & VertexBufferCreateFlag_Writable) {
        return new VulkanIndexBuffer<uint16_t, uint16_t, WritableIndexBuffer>(this);
    } else {
        return new VulkanIndexBuffer<uint16_t, uint16_t, ReadOnlyIndexBuffer>(this);
    }
}

IndexBuffer<uint8_t>* VulkanRenderer::make_index_buffer_8(IndexBufferCreateFlags flags) {
    if (flags & VertexBufferCreateFlag_Writable) {
        return new VulkanIndexBuffer<uint8_t, uint16_t, WritableIndexBuffer>(this);
    } else {
        return new VulkanIndexBuffer<uint8_t, uint16_t, ReadOnlyIndexBuffer>(this);
    }
}


UniformBuffer* VulkanRenderer::make_uniform_buffer() {
    return new VulkanUniformBuffer(this);
}

Shader* VulkanRenderer::make_shader() {
    return new VulkanShader(this);
}

Texture* VulkanRenderer::make_texture() {
    return new VulkanTexture(this);
}

Sampler* VulkanRenderer::make_sampler() {
    return new VulkanSampler(this);
}

DescriptorPool* VulkanRenderer::make_descriptor_pool() {
    return new VulkanDescriptorPool(this);
}

DescriptorSet* VulkanRenderer::make_descriptor_set() {
    return new VulkanDescriptorSet(this);
}

RenderPass* VulkanRenderer::make_render_pass() {
    return new VulkanRenderPass(this);
}

Framebuffer *VulkanRenderer::make_framebuffer() {
    return new VulkanFramebuffer(this);
}

Pipeline *VulkanRenderer::make_pipeline() {
    return new VulkanPipeline(this);
}

const RenderTarget *VulkanRenderer::swapchain() const {
    return _swapchain.rendertarget();
}

uint32_t VulkanRenderer::next_submission_index() const {
    return _submissions.back();
}

uint32_t VulkanRenderer::swapchain_image_count() const {
    return _swapchain.image_count();
}

void VulkanRenderer::submit(const PassSubmissionInfo *passes, uint32_t pass_count) {
    assert(validation::validate_submission_passes(passes, pass_count, RendererType::Vulkan));

    uint32_t submission = _submissions.back();

    uint32_t next_image;
    vkAcquireNextImageKHR(_device, _swapchain.handle(), 0, _swapchain_image_available_semaphores[submission], nullptr, &next_image);
    _image_indices_by_submission[submission] = next_image;

    record_command_buffers(passes, pass_count, submission, next_image);

    VkSubmitInfo submit_info = {};
    VkSemaphore wait_semaphore = _swapchain_image_available_semaphores[submission];
    VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = &wait_semaphore;
    submit_info.pWaitDstStageMask = &wait_stage;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &_command_buffer_handles_by_submission[submission];

    VkFence fence = _fences_by_submission[submission];
    vkQueueSubmit(_graphics_queue, 1, &submit_info, fence);

    // Wait for the oldest command buffers to finsh execution.
    uint32_t oldest_submission = _submissions.front();
    _submissions.pop();
    _submissions.push(oldest_submission);

    VkFence oldest_submission_fence = _fences_by_submission[oldest_submission];
    vkWaitForFences(_device, 1, &oldest_submission_fence, VK_TRUE, numeric_limits<uint64_t>::max());
    vkResetFences(_device, 1, &oldest_submission_fence);

    //
    // Present the previous frame
    //
    VkPresentInfoKHR present_info = {};
    VkSwapchainKHR swapchain_handle = _swapchain.handle();
    //VkResult present_result = VK_RESULT_MAX_ENUM;
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.swapchainCount = 1;
    present_info.pSwapchains = &swapchain_handle;
    present_info.pImageIndices = &next_image;
    // TODO: This was commented out for some reason, but I can't remember why. Need to look into this again.
    //present_info.pResults = &present_result;
    vkQueuePresentKHR(_present_queue, &present_info);

    // We can now delete the resources associated with this submission
    delete_resources_for_image_index(oldest_submission);
}

void VulkanRenderer::record_command_buffers(const PassSubmissionInfo *passes, uint32_t pass_count, uint32_t submission, uint32_t image_index) {
    VulkanCommandPool &pool = _command_pools_by_submission[submission];
    VulkanCommandBuffer &buffer = _command_buffers_by_submission[submission];

    pool.reset_buffers();
    buffer.record(passes, pass_count, image_index);
}

VkPhysicalDevice VulkanRenderer::physical_device() const {
    return _physical_device;
}

VkDevice VulkanRenderer::device() const {
    return _device;
}

const QueueFamilyIndices& VulkanRenderer::queue_family_indices() const {
    return _queue_family_indices;
}

VkCommandPool VulkanRenderer::copy_command_pool() const {
    return _copy_command_pool.handle();
}

VkQueue VulkanRenderer::graphics_queue() const {
    return _graphics_queue;
}

bool VulkanRenderer::find_memory_type(
    uint32_t type_filter,
    VkMemoryPropertyFlags properties,
    uint32_t &found_memory_type
) const {
    for (uint32_t i = 0; i < _memory_properties.memoryTypeCount; ++i) {
        if (type_filter & (1 << i)
            && _memory_properties.memoryTypes[i].propertyFlags & properties
        ) {
            found_memory_type = i;
            return true;
        }
    }
    return false;
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

    // TODO: Need mechanism for turning validation layers on or off.
    //array<const char *, 0> validation_layers = {};

    array<const char *, 1> validation_layers = {
        "VK_LAYER_LUNARG_standard_validation"
    };

    // Validation layers
    create_info.enabledLayerCount = validation_layers.size();
    create_info.ppEnabledLayerNames = validation_layers.data();

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
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

    unique_ptr<VkPhysicalDevice[]> devices(new VkPhysicalDevice[device_count]);
    vkEnumeratePhysicalDevices(instance, &device_count, devices.get());

    for (uint32_t i = 0; i < device_count; ++i) {
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
    const SwapchainInfo &/*info*/
) {
    // VK_PRESENT_MODE_FIFO_KHR is guaranteed to always be available.
    VkPresentModeKHR selected_mode = VK_PRESENT_MODE_FIFO_KHR;
    //VkPresentModeKHR selected_mode = VK_PRESENT_MODE_IMMEDIATE_KHR;

    // TODO: Make this smarter and capable of choosing different modes.
//    for (unsigned int i = 0; i < info.present_mode_count; ++i) {
//        VkPresentModeKHR mode = info.present_modes[i];
//        if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
//            return mode;
//        }
//    }

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

void VulkanRenderer::create_buffer(
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags memory_properties,
    VkBuffer &buffer,
    VkDeviceMemory &device_memory
) const {
    VkBufferCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    create_info.size = size;
    create_info.usage = usage;
    create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    if (vkCreateBuffer(_device, &create_info, nullptr, &buffer) != VK_SUCCESS) {
        return;
    }

    VkMemoryRequirements memory_requirements;
    vkGetBufferMemoryRequirements(_device, buffer, &memory_requirements);
    uint32_t memory_type_index;
    if (!find_memory_type(
        memory_requirements.memoryTypeBits,
        memory_properties,
        memory_type_index
    )) {
        return;
    }

    VkDeviceSize actual_size = max(memory_requirements.size, size);

    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = actual_size;
    alloc_info.memoryTypeIndex = memory_type_index;
    if (vkAllocateMemory(_device, &alloc_info, nullptr, &device_memory) != VK_SUCCESS) {
        return;
    }



    vkBindBufferMemory(_device, buffer, device_memory, 0);
}

VkResult VulkanRenderer::copy_buffer(VkBuffer src, VkBuffer dest, VkDeviceSize size) const {
    VkCommandBufferAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandPool = _copy_command_pool.handle();
    alloc_info.commandBufferCount = 1;

    VkCommandBuffer command_buffer;
    VkResult result = vkAllocateCommandBuffers(_device, &alloc_info, &command_buffer);
    if (result != VK_SUCCESS) {
        return result;
    }

    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(command_buffer, &begin_info);

    VkBufferCopy region = {};
    region.srcOffset = 0;
    region.dstOffset = 0;
    region.size = size;
    vkCmdCopyBuffer(command_buffer, src, dest, 1, &region);

    vkEndCommandBuffer(command_buffer);

    VkSubmitInfo submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer;

    VkFenceCreateInfo fence_info = {};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = 0;
    fence_info.pNext = nullptr;

    VkFence fence;
    vkCreateFence(_device, &fence_info, nullptr, &fence);

    vkQueueSubmit(_graphics_queue, 1, &submit_info, fence);

    result = VK_TIMEOUT;
    while (result == VK_TIMEOUT) {
        result = vkWaitForFences(_device, 1, &fence, VK_FALSE, numeric_limits<uint64_t>::max());
    }

    vkDestroyFence(_device, fence, nullptr);
    vkFreeCommandBuffers(_device, _copy_command_pool.handle(), 1, &command_buffer);

    return result;
}

void VulkanRenderer::delete_when_safe(unique_ptr<SwapchainSafeDeleteable> deleteable) {
    vector<unique_ptr<SwapchainSafeDeleteable>> &list_for_next_swapchain_image =
            _safe_deletables_by_image_index[next_submission_index()];

    list_for_next_swapchain_image.emplace_back(move(deleteable));
}

void VulkanRenderer::delete_resources_for_image_index(uint32_t image_index) {
    vector<unique_ptr<SwapchainSafeDeleteable>> &list_for_next_swapchain_image =
        _safe_deletables_by_image_index[image_index];

    for (unique_ptr<SwapchainSafeDeleteable> &deletable : list_for_next_swapchain_image) {
        deletable->delete_resources(*this);
    }

    list_for_next_swapchain_image.clear();
}


void VulkanRenderer::finish_enqueued_command_buffers() {
    // Wait on all fences which represent command buffer executions in progress. Don't wait on the last fence in the
    // queue, since it represents the next queue submission which hasn't happened yet.
    while (_submissions.size() > 1) {
        uint32_t submission = _submissions.front();
        _submissions.pop();
        VkFence fence = _fences_by_submission[submission];
        vkWaitForFences(_device, 1, &fence, /* wait all? */ VK_TRUE, /* timeout */ numeric_limits<uint64_t>::max());
    }
}

VkFormat VulkanRenderer::translate_texture_format(TextureFormat format) {
    switch (format) {
        case TextureFormat::RGB:
            return VK_FORMAT_R8G8B8_UNORM;
        case TextureFormat::RGBA:
            return VK_FORMAT_R8G8B8A8_UNORM;
        case TextureFormat::Depth16:
            return VK_FORMAT_D16_UNORM;
        case TextureFormat::Depth24:
            return VK_FORMAT_X8_D24_UNORM_PACK32;
        case TextureFormat::Depth32:
            return VK_FORMAT_UNDEFINED;
        case TextureFormat::Depth32Float:
            return VK_FORMAT_D32_SFLOAT;
    }
    return VK_FORMAT_UNDEFINED;
}
