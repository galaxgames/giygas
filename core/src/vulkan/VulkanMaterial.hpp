#pragma once
#include <giygas/Material.hpp>

namespace giygas {

    class VulkanRenderer;

    class VulkanMaterial : public Material {

        VulkanRenderer *_renderer;

        void move_common(VulkanMaterial &&) noexcept;

    public:
        explicit VulkanMaterial(VulkanRenderer *renderer);
        VulkanMaterial(const VulkanMaterial &) = delete;
        VulkanMaterial &operator=(const VulkanMaterial &) = delete;
        VulkanMaterial(VulkanMaterial &&) noexcept;
        VulkanMaterial &operator=(VulkanMaterial &&) noexcept;
        ~VulkanMaterial() override;

        //
        // Material implementation
        //

        RendererType renderer_type() const override;
        void link_shaders(const Shader **shaders, size_t count) override;

        void set_textures(
            const std::weak_ptr<Texture> *textures,
            size_t count
        ) override;


        //
        // VulkanMaterial implementation
        //



    };


}