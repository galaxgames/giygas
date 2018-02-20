#include "VulkanSampler.hpp"
#include "VulkanRenderer.hpp"

using namespace giygas;

VulkanSampler::VulkanSampler(VulkanRenderer *renderer) {
    _renderer = renderer;
    _handle = VK_NULL_HANDLE;
}

VulkanSampler::~VulkanSampler() {
    vkDestroySampler(_renderer->device(), _handle, nullptr);
}

RendererType VulkanSampler::renderer_type() const {
    return RendererType::Vulkan;
}

void VulkanSampler::create(const SamplerParameters &params) {
    VkSamplerCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    create_info.minFilter = translate_filter(params.minify_filter_mode);
    create_info.magFilter = translate_filter(params.magnify_filter_mode);
    create_info.addressModeU = wrap_to_address_mode(params.wrap_mode_u);
    create_info.addressModeV = wrap_to_address_mode(params.wrap_mode_v);
    create_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    create_info.anisotropyEnable = VK_FALSE;
    create_info.maxAnisotropy = 1;
    create_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    create_info.unnormalizedCoordinates = VK_FALSE;
    create_info.compareEnable = VK_FALSE;
    create_info.compareOp = VK_COMPARE_OP_ALWAYS;
    create_info.mipmapMode = translate_mipmap_mode(params.mipmap_mode);
    create_info.mipLodBias = 0;
    create_info.minLod = 0;
    create_info.maxLod = 0;

    vkCreateSampler(_renderer->device(), &create_info, nullptr, &_handle);
}

VkSampler VulkanSampler::handle() const {
    return _handle;
}

VkFilter VulkanSampler::translate_filter(SamplerFilterMode mode) {
    switch (mode) {
        case SamplerFilterMode::Nearest:
            return VK_FILTER_NEAREST;
        case SamplerFilterMode::Linear:
            return VK_FILTER_LINEAR;
    }
}

VkSamplerAddressMode VulkanSampler::wrap_to_address_mode(SamplerWrapMode mode) {
    switch (mode) {
        case SamplerWrapMode::ClampToEdge:
            return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        case SamplerWrapMode::ClampToBorder:
            return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
        case SamplerWrapMode::MirroredRepeat:
            return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
        case SamplerWrapMode::Repeat:
            return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    }
}

VkSamplerMipmapMode VulkanSampler::translate_mipmap_mode(SamplerMipmapMode mode) {
    switch (mode) {
        case SamplerMipmapMode::Nearest:
            return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        case SamplerMipmapMode::Linear:
            return VK_SAMPLER_MIPMAP_MODE_LINEAR;
    }
}