#pragma once
#include <giygas/Sampler.hpp>
#include <vulkan/vulkan.h>
#include <giygas/SamplerParameters.hpp>
#include <giygas/RendererType.hpp>

namespace giygas {

    class VulkanRenderer;

    class VulkanSampler final : public Sampler {

        VulkanRenderer *_renderer;
        VkSampler _handle;

        static VkFilter translate_filter(SamplerFilterMode mode);
        static VkSamplerAddressMode wrap_to_address_mode(SamplerWrapMode mode);
        static VkSamplerMipmapMode translate_mipmap_mode(SamplerMipmapMode mode);

    public:
        VulkanSampler(VulkanRenderer *renderer);
        VulkanSampler(const VulkanSampler &) = delete;
        VulkanSampler &operator=(const VulkanSampler &) = delete;
        VulkanSampler(VulkanSampler &&) = delete;
        VulkanSampler &operator=(VulkanSampler &&) = delete;
        ~VulkanSampler() override;

        //
        // Sampler implementation
        //

        RendererType renderer_type() const override;
        void create(const SamplerParameters &params) override;


        //
        // VulkanSampler implementation
        //

        VkSampler handle() const;


    };

}