#include <algorithm>
#include "VulkanTexture.hpp"
#include "VulkanRenderer.hpp"

using namespace giygas;

VulkanTexture::VulkanTexture(VulkanRenderer *renderer) {
    _renderer = renderer;
    _image = VK_NULL_HANDLE;
    _image_view = VK_NULL_HANDLE;
    _image_memory = VK_NULL_HANDLE;
}

VulkanTexture::~VulkanTexture() {

}

RendererType VulkanTexture::renderer_type() const {
    return RendererType::Vulkan;
}

void VulkanTexture::set_data(
    unique_ptr<uint8_t[]> &&data,
    size_t size,
    uint32_t width,
    uint32_t height,
    TextureFormat format
) {
    VkDevice device = _renderer->device();

    _data = move(data);
    _size = size;
    _width = width;
    _height = height;
    _format = format;

    VkFormat translated_format = translate_format(format);

    VkBuffer staging_buffer;
    VkDeviceMemory staging_buffer_memory;

    _renderer->create_buffer(
        size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        staging_buffer,
        staging_buffer_memory
    );

    void *mapped_data;
    vkMapMemory(device, staging_buffer_memory, 0, size, 0, &mapped_data);
    copy_n(data.get(), size, static_cast<uint8_t *>(mapped_data));
    vkUnmapMemory(device, staging_buffer_memory);

    create_image(
        width,
        height,
        translated_format,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        _image,
        _image_memory
    );

    transition_image_layout(
        _image,
        translated_format,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
    );

    copy_buffer_to_image(staging_buffer, _image, width, height);

    transition_image_layout(
        _image,
        translated_format,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    );

}

void VulkanTexture::create_storage(
    uint32_t width,
    uint32_t height,
    TextureFormat format
) {
    // TODO
}

const void* VulkanTexture::impl() const {
    return static_cast<const VulkanRenderTarget *>(this);
}

VkImageView VulkanTexture::image_view() const {
    return _image_view;
}

void VulkanTexture::create_image(
    uint32_t width,
    uint32_t height,
    VkFormat format,
    VkImageTiling tiling,
    VkImageUsageFlags usage_flags,
    VkMemoryPropertyFlags memory_properties,
    VkImage &image,
    VkDeviceMemory &image_memory
) const {
    VkDevice device = _renderer->device();

    VkImageCreateInfo image_create_info = {};
    image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_create_info.imageType = VK_IMAGE_TYPE_2D;
    image_create_info.extent.width = width;
    image_create_info.extent.height = height;
    image_create_info.extent.depth = 1;
    image_create_info.mipLevels = 1;
    image_create_info.arrayLayers = 1;
    image_create_info.format = format;
    image_create_info.tiling = tiling;
    image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_create_info.usage = usage_flags;
    image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;  // Don't need to share with other queues
    image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
    image_create_info.flags = 0;

    vkCreateImage(device, &image_create_info, nullptr, &image);

    VkMemoryRequirements memory_requirements;
    vkGetImageMemoryRequirements(device, image, &memory_requirements);

    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = memory_requirements.size;
    if (!_renderer->find_memory_type(
        memory_requirements.memoryTypeBits,
        memory_properties,
        alloc_info.memoryTypeIndex
    )) {
        return;
    }

    vkAllocateMemory(device, &alloc_info, nullptr, &image_memory);
    vkBindImageMemory(device, image, image_memory, 0);
}

void VulkanTexture::transition_image_layout(
    VkImage image,
    VkFormat format,
    VkImageLayout old_layout,
    VkImageLayout new_layout
) const {
    VkCommandBuffer command_buffer = begin_command_buffer();

    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = old_layout;
    barrier.newLayout = new_layout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.srcAccessMask = 0;  // todo
    barrier.dstAccessMask = 0;  // todo

    vkCmdPipelineBarrier(
        command_buffer,
        0,  // src stage mask // todo
        0,  // dst stage mask // todo
        0,  // dependency flags
        0,  // memory barrier count
        nullptr,  // memory barriers
        0,  // buffer memory barrier count
        nullptr,  // buffer memory barriers
        1,  // image memory barrier count
        &barrier
    );

    end_command_buffer(command_buffer);
}

void VulkanTexture::copy_buffer_to_image(
    VkBuffer buffer,
    VkImage image,
    uint32_t width,
    uint32_t height
) {
    VkCommandBuffer command_buffer = begin_command_buffer();

    VkBufferImageCopy region = {};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {width, height, 1 };

    vkCmdCopyBufferToImage(
        command_buffer,
        buffer,
        image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region
    );

    end_command_buffer(command_buffer);
}

VkCommandBuffer VulkanTexture::begin_command_buffer() const {
    VkCommandPool pool = _renderer->copy_command_pool();

    VkCommandBufferAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandPool = pool;
    alloc_info.commandBufferCount = 1;

    VkCommandBuffer command_buffer;
    vkAllocateCommandBuffers(_renderer->device(), &alloc_info, &command_buffer);

    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(command_buffer, &begin_info);
    return command_buffer;
}

void VulkanTexture::end_command_buffer(VkCommandBuffer buffer) const {
    VkDevice device = _renderer->device();
    VkCommandPool pool = _renderer->copy_command_pool();

    vkEndCommandBuffer(buffer);

    VkSubmitInfo submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &buffer;

    VkFenceCreateInfo fence_info = {};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_info.flags = 0;

    VkFence fence;
    vkCreateFence(device, &fence_info, nullptr, &fence);

    vkQueueSubmit(_renderer->graphics_queue(), 1, &submit_info, fence);

    VkResult result = VK_TIMEOUT;
    while (result == VK_TIMEOUT) {
        // 1e9 -> 1 second timeout
        result = vkWaitForFences(device, 1, &fence, VK_FALSE, 1000000000);
    }

    vkDestroyFence(device, fence, nullptr);
    vkFreeCommandBuffers(_renderer->device(), pool, 1, &buffer);
}

VkFormat VulkanTexture::translate_format(TextureFormat format) {
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
}
