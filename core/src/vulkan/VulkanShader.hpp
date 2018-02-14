#pragma once
#include <giygas/Shader.hpp>
#include <vulkan/vulkan.h>
#include <array>


namespace giygas {

    using namespace std;

    class VulkanRenderer;

    class VulkanShader final : public Shader {
        const VulkanRenderer *_renderer;
        VkShaderModule _module;
        ShaderType _type;

        void move_common(VulkanShader &&other) noexcept;

    public:
        explicit VulkanShader(const VulkanRenderer *renderer);
        VulkanShader(const VulkanShader &) = delete;
        VulkanShader &operator=(const VulkanShader &) = delete;
        VulkanShader(VulkanShader &&) noexcept;
        VulkanShader &operator=(VulkanShader &&) noexcept;
        ~VulkanShader() override;

        //
        // Shader implementation
        //

        RendererType renderer_type() const override;

        void set_code(
            const uint8_t* vertex, size_t length, ShaderType type
        ) override;

        ShaderType shader_type() const override;


        //
        // VulkanShader implementation
        //

        VkShaderModule module() const;

    };

}