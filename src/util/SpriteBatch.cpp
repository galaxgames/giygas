#include <giygas/util/SpriteBatch.hpp>

using namespace giygas;
using namespace std;

SpriteBatch::SpriteBatch(Renderer &renderer) :
    _vao(renderer.make_vao()),
    _vbo(renderer.make_vbo()),
    _ebo(renderer.make_int_ebo())
{
    _count = 0;

    VertexBufferLayout layout;
    layout.set_channel(0, 2); // position
    layout.set_channel(1, 2); // uvs
    layout.set_channel(1, 3); // color
    _vao->add_buffer(_vbo.get(), layout);
}

SpriteBatch::SpriteBatch(SpriteBatch &&) noexcept = default;
SpriteBatch& SpriteBatch::operator=(SpriteBatch &&) noexcept = default;

SpriteBatch::~SpriteBatch() = default;

void SpriteBatch::set_material(SpriteBatchMaterial mat) {
    _mat = mat;
}

void SpriteBatch::begin() {
    _count = 0;
}

void SpriteBatch::end() {
    size_t offset = 0;
    for (const auto &sprite : _sprites) {
        append_verts_for_sprite(sprite, offset);
        offset += COMPONENTS_PER_SPRITE;
    }
    set_elements(_count);
}

void SpriteBatch::draw(Renderer &renderer) const {
    if (auto mat = _mat.material.lock()) {
        renderer.draw(
            _vao.get(),
            _ebo.get(),
            mat.get(),
            _ebo->count()
        );
    }
}

void SpriteBatch::add(SpriteInfo info) {
    ++_count;
    while (_sprites.size() < _count) {
        _sprites.emplace_back();
    }
    _sprites[_count - 1] = info;
}

void SpriteBatch::append_verts_for_sprite(const SpriteInfo &info, size_t offset) {
    float minX = info.position.x;
    float minY = info.position.y;
    float maxX = minX + info.size.x;
    float maxY = minY + info.size.y;

    float data[COMPONENTS_PER_SPRITE] = {
        // pos,      uv,    color
        minX, minY,  0, 0,  1, 1, 1,
        maxX, minY,  1, 0,  1, 1, 1,
        minX, maxY,  0, 1,  1, 1, 1,
        maxX, maxY,  1, 1,  1, 1, 1
    };

    _vbo->set_data(offset, data, COMPONENTS_PER_SPRITE);
}

void SpriteBatch::set_elements(size_t sprite_count) {
    auto sprites_in_ebo = _ebo->count() / ELEMENTS_PER_SPRITE;
    if (sprite_count > sprites_in_ebo) {
        size_t offset = sprites_in_ebo * ELEMENTS_PER_SPRITE;
        unsigned int next_index = static_cast<unsigned int>(sprites_in_ebo * VERTS_PER_SPRITE);
        while (sprites_in_ebo < sprite_count) {
            unsigned int data[ELEMENTS_PER_SPRITE] = {
                next_index, next_index + 1, next_index + 2,
                next_index + 2, next_index + 1, next_index + 3
            };
            _ebo->set(offset, data, ELEMENTS_PER_SPRITE);
            offset += ELEMENTS_PER_SPRITE;
            next_index += VERTS_PER_SPRITE;
            ++sprites_in_ebo;
        }
    }
}
