#pragma once
#include <giygas/Sampler.hpp>
#include <vulkan/vulkan.h>
#include <giygas/SamplerOptions.hpp>

namespace giygas {

    class VulkanRenderer;

    class VulkanSampler final : public Sampler {

        VulkanRenderer *_renderer;
        VkSampler _handle;

        static VkFilter translate_filter(SamplerFilterMode mode);
        static VkSamplerAddressMode wrap_to_address_mode(SamplerWrapMode mode);
        static VkSamplerMipmapMode translate_mipmap_mode(SamplerMipmapMode mode);

    public:
        VulkanSampler(VulkanRenderer *renderer, const SamplerOptions &options);
        VulkanSampler(const VulkanSampler &) = delete;
        VulkanSampler &operator=(const VulkanSampler &) = delete;
        VulkanSampler(VulkanSampler &&) = delete;
        VulkanSampler &operator=(VulkanSampler &&) = delete;
        ~VulkanSampler() override;

        //
        // VulkanSampler implementation
        //


    };

}