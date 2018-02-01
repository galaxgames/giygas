#pragma once
#include <giygas/Material.hpp>

namespace giygas {

    class VulkanRenderer;

    class VulkanMaterial final : public Material {

        VulkanRenderer *_renderer;


    public:
        explicit VulkanMaterial(VulkanRenderer *renderer);
        VulkanMaterial(const VulkanMaterial &) = delete;
        VulkanMaterial &operator=(const VulkanMaterial &) = delete;
        VulkanMaterial(VulkanMaterial &&) noexcept = delete;
        VulkanMaterial &operator=(VulkanMaterial &&) noexcept = delete;
        ~VulkanMaterial() override;

        //
        // Material implementation
        //

        RendererType renderer_type() const override;
        void set_textures(const Texture **textures, size_t count) override;
        void set_uniform_float(size_t location, float value) override;
        void set_uniform_texture(size_t location, size_t value) override;
        void set_uniform_vector2(size_t location, Vector2 value) override;
        void set_uniform_vector3(size_t location, Vector3 value) override;
        void set_uniform_vector4(size_t location, Vector4 value) override;
        void set_uniform_matrix4x4(size_t location, const Matrix4x4 &value) override;


        //
        // VulkanMaterial implementation
        //



    };


}