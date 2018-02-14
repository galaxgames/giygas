#pragma once

#include <memory>
#include <vector>
#include <string>
#include <array>
#include "SpriteInfo.hpp"
#include <giygas/Texture.hpp>
#include <giygas/VertexBuffer.hpp>
#include <giygas/IndexBuffer.hpp>
#include <giygas/Renderer.hpp>
#include <giygas/Material.hpp>
#include <giygas/IndexRange.hpp>

namespace giygas {

    class SpriteBatchMaterial {
    public:
        std::weak_ptr<Material> material;
        size_t texture_location;
    };

    class SpriteBatch {
        std::vector<SpriteInfo> _sprites;
        std::vector<std::vector<size_t>> _sprites_by_texture;
        std::vector<Texture *> _textures;
        std::vector<IndexRange> _draw_call_details;
        std::unique_ptr<VertexBuffer> _vbo;
        std::unique_ptr<IndexBuffer32> _ebo;
        SpriteBatchMaterial _mat;
        unsigned int _count;

        static const size_t COMPONENTS_PER_VERT = 8;
        static const size_t VERTS_PER_SPRITE = 4;
        static const size_t COMPONENTS_PER_SPRITE
            = COMPONENTS_PER_VERT * VERTS_PER_SPRITE;
        static const size_t ELEMENTS_PER_SPRITE = 6;

        void append_verts_for_sprite(const SpriteInfo &info, size_t offset);
        void set_elements();
        void set_elements_for_texture(
            const std::vector<size_t> &sprite_indices, unsigned int *elements
        );

    public:
        SpriteBatch(Renderer &renderer);
        SpriteBatch(const SpriteBatch &) = delete;
        SpriteBatch(SpriteBatch &&) noexcept;
        SpriteBatch &operator=(const SpriteBatch &) = delete;
        SpriteBatch &operator=(SpriteBatch &&) noexcept;
        virtual ~SpriteBatch();

        void set_material(SpriteBatchMaterial mat);
        void set_textures(Texture **textures, size_t count);
        void begin();
        void end();
        //void draw(Surface &surface) const;
        void add(SpriteInfo info);

    };
}
