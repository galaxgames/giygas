#include <algorithm>
#include <giygas/giygas.hpp>
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
    VkDevice device = _renderer->device();
    vkDestroyImageView(device, _image_view, nullptr);
    vkDestroyImage(device, _image, nullptr);
    vkFreeMemory(device, _image_memory, nullptr);
}

RendererType VulkanTexture::renderer_type() const {
    return RendererType::Vulkan;
}

//void VulkanTexture::create(
//    unique_ptr<uint8_t[]> &&data,
//    size_t size,
//    uint32_t width,
//    uint32_t height,
//    TextureFormat format
//) {
//    create(move(data), size, width, height, format, VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
//}

void VulkanTexture::create(
    unique_ptr<uint8_t[]> &&data,
    size_t size,
    uint32_t width,
    uint32_t height,
    TextureFormat input_format,
    TextureFormat desired_format,
    TextureUsageFlags usage
) {
    assert(_image == VK_NULL_HANDLE);

    VkDevice device = _renderer->device();

    TextureFormat format = input_format; // TODO

    _data = move(data);
    _size = size;
    _width = width;
    _height = height;
    _format = format;

    // Figure out the desired layout and usage flags
    VkImageLayout final_layout = VK_IMAGE_LAYOUT_UNDEFINED;
    VkImageUsageFlags usage_flags = 0;
    VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
    if (usage & TEXTURE_USAGE_SAMPLE) {
        final_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        usage_flags |= VK_IMAGE_USAGE_SAMPLED_BIT;
    }
    if (usage & TEXTURE_USAGE_DEPTH_ATTACHMENT || usage & TEXTURE_USAGE_STENCIL_ATTACHMENT) {
        final_layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        usage_flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    }
    if (usage & TEXTURE_USAGE_COLOR_ATTACHMENT) {
        final_layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        usage_flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    }
    _layout = final_layout;

    // TODO: FIX THIS: We don't check VkFormatProperties::optimalTilingFeatures from physical device
    // https://www.khronos.org/registry/vulkan/specs/1.0/html/vkspec.html#VUID-VkImageCreateInfo-tiling-00985



    VkFormat translated_format = VulkanRenderer::translate_texture_format(format);
    _api_format = translated_format;

    VkImageLayout current_layout = VK_IMAGE_LAYOUT_UNDEFINED;

    if (size > 0) {
        usage_flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    }

    create_image(
        width,
        height,
        translated_format,
        tiling,
        usage_flags,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        _image,
        _image_memory,
        current_layout
    );

    if (size > 0) {
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

        VkImageLayout next_layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        transition_image_layout(
            _image,
            format,
            current_layout,
            next_layout
        );
        current_layout = next_layout;

        copy_buffer_to_image(staging_buffer, _image, width, height);

        vkFreeMemory(device, staging_buffer_memory, nullptr);
        vkDestroyBuffer(device, staging_buffer, nullptr);
    }

    transition_image_layout(
        _image,
        format,
        current_layout,
        final_layout
    );

    VkImageViewCreateInfo view_info;
    view_info = {};
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format = translated_format;
    view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    view_info.subresourceRange.aspectMask = image_aspects_from_format(format);
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;
    view_info.image = _image;

    vkCreateImageView(device, &view_info, nullptr, &_image_view);
}

TextureFormat VulkanTexture::format() const {
    return _format;
}

const void* VulkanTexture::rendertarget_impl() const {
    return static_cast<const VulkanRenderTarget *>(this);
}

const void * VulkanTexture::texture_impl() const {
    return static_cast<const VulkanTexture *>(this);
}

VkImageView VulkanTexture::image_view() const {
    return _image_view;
}

VkImageLayout VulkanTexture::layout() const {
    return _layout;
}

VkFormat VulkanTexture::api_format() const {
    return _api_format;
}

void VulkanTexture::create_image(
    uint32_t width,
    uint32_t height,
    VkFormat format,
    VkImageTiling tiling,
    VkImageUsageFlags usage_flags,
    VkMemoryPropertyFlags memory_properties,
    VkImage &image,
    VkDeviceMemory &image_memory,
    VkImageLayout initial_layout
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
    image_create_info.initialLayout = initial_layout;
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
    TextureFormat format,
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
    barrier.subresourceRange.aspectMask = image_aspects_from_format(format);
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags source_stage;
    VkPipelineStageFlags dest_stage;

    if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED) {
        barrier.srcAccessMask = 0;
        source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    }
    else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else {
        // Giygas bug
        assert(false);
    }

    if (new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        dest_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        dest_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
        dest_stage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    }
    else if (new_layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
        barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
        dest_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    }
    else {
        // Giygas bug
        assert(false);
    }

    vkCmdPipelineBarrier(
        command_buffer,
        source_stage,
        dest_stage,
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
        result = vkWaitForFences(device, 1, &fence, VK_FALSE, numeric_limits<uint64_t>::max());
    }

    vkDestroyFence(device, fence, nullptr);
    vkFreeCommandBuffers(_renderer->device(), pool, 1, &buffer);
}

VkImageAspectFlags VulkanTexture::image_aspects_from_format(TextureFormat format) {
    switch (attachment_purpose_from_texture_format(format)) {
        case AttachmentPurpose::Color:
            return VK_IMAGE_ASPECT_COLOR_BIT;
        case AttachmentPurpose::DepthStencil:
            if (is_depth_format(format)) {
                return VK_IMAGE_ASPECT_DEPTH_BIT;
            }
            return VK_IMAGE_ASPECT_STENCIL_BIT;
    }
}
