#pragma once
#include <giygas/Material.hpp>

namespace giygas {
    using namespace std;

    class VulkanRenderer;

    class VulkanMaterial final : public Material {

        class TextureData {
        public:
            const Texture *texture;
            const Sampler *sampler;
        };

        VulkanRenderer *_renderer;
        unique_ptr<TextureData[]> _textures;
        size_t _texture_count;
        unique_ptr<uint8_t[]> _data;
        size_t _data_size;

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

        void set_textures(
            const Texture **textures,
            const Sampler **samplers,
            size_t count
        ) override;

        void reset(size_t size) override;
        void set_uniform_float(size_t location, float value) override;
        void set_uniform_texture(size_t location, uint32_t value) override;
        void set_uniform_vector2(size_t location, Vector2 value) override;
        void set_uniform_vector3(size_t location, Vector3 value) override;
        void set_uniform_vector4(size_t location, Vector4 value) override;
        void set_uniform_matrix4x4(size_t location, const Matrix4x4 &value) override;


        //
        // VulkanMaterial implementation
        //



    };


}