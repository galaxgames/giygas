#pragma once

#include <memory>
#include <vector>
#include <string>
#include <array>
#include <giygas/Vector2.hpp>
#include <giygas/Texture.hpp>
#include <giygas/VertexBuffer.hpp>
#include <giygas/ElementBuffer.hpp>
#include <giygas/Renderer.hpp>
#include <giygas/Material.hpp>

namespace giygas {

    class SpriteInfo {
    public:
        Vector2 position;
        Vector2 size;
        Vector3 color;
        std::weak_ptr<Texture> texture;
    };

    class SpriteBatchMaterial {
    public:
        std::weak_ptr<Material> material;
        std::string texture_uniform_name;
    };

    class SpriteBatch {
        std::vector<SpriteInfo> _sprites;
        std::unique_ptr<VertexArray> _vao;
        std::unique_ptr<VertexBuffer> _vbo;
        std::unique_ptr<ElementBufferInt> _ebo;
        SpriteBatchMaterial _mat;
        unsigned int _count;

        static const size_t COMPONENTS_PER_VERT = 7;
        static const size_t VERTS_PER_SPRITE = 4;
        static const size_t COMPONENTS_PER_SPRITE
            = COMPONENTS_PER_VERT * VERTS_PER_SPRITE;
        static const size_t ELEMENTS_PER_SPRITE = 6;

        void append_verts_for_sprite(const SpriteInfo &info, size_t offset);
        void set_elements(size_t sprite_count);

    public:
        SpriteBatch(Renderer &renderer);
        SpriteBatch(const SpriteBatch &) = delete;
        SpriteBatch(SpriteBatch &&) noexcept;
        SpriteBatch &operator=(const SpriteBatch &) = delete;
        SpriteBatch &operator=(SpriteBatch &&) noexcept;
        virtual ~SpriteBatch();

        void set_material(SpriteBatchMaterial mat);
        void begin();
        void end();
        void draw(Renderer &renderer) const;
        void add(SpriteInfo info);

    };
}
