#include <giygasutil/SpriteBatch.hpp>
#include <cassert>

using namespace giygas;
using namespace std;

SpriteBatch::SpriteBatch(Renderer &renderer) :
    _vbo(renderer.make_vertex_buffer(VertexBufferCreateFlag_None)),
    _ebo(renderer.make_index_buffer_32())
{
    _count = 0;

//    VertexAttributeLayout layout(3, 32); // 3 attribs, stride 32
//    layout.add_attribute(2, 4, 0); // position
//    layout.add_attribute(2, 4, 8); // uvs
//    layout.add_attribute(4, 4, 16); // color
//    _vao->add_buffer(_vbo.get(), layout);
}

SpriteBatch::SpriteBatch(SpriteBatch &&) noexcept = default;
SpriteBatch& SpriteBatch::operator=(SpriteBatch &&) noexcept = default;

SpriteBatch::~SpriteBatch() = default;

//void SpriteBatch::set_material(SpriteBatchMaterial mat) {
//    _mat = mat;
//}

void SpriteBatch::set_textures(Texture **textures, size_t count) {
    assert(_sprites.size() == 0);
    _textures.clear();
    _textures.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        _textures.push_back(textures[i]);
    }
    _sprites_by_texture.clear();
    _sprites_by_texture.resize(count);
}

void SpriteBatch::begin() {
    _count = 0;
    _sprites.clear();
    for (size_t i = 0, ilen = _sprites_by_texture.size(); i < ilen; ++i) {
        _sprites_by_texture[i].clear();
    }
}

void SpriteBatch::end() {
    size_t offset = 0;
    for (const auto &sprite : _sprites) {
        append_verts_for_sprite(sprite, offset);
        offset += COMPONENTS_PER_SPRITE;
    }
    set_elements();
}

//void SpriteBatch::draw(Surface &surface) const {
//    if (auto mat = _mat.material.lock()) {
//        for (size_t i = 0, ilen = _sprites_by_texture.size(); i < ilen; ++i) {
//            const Texture *texture = _textures[i];
//            //mat->set_textures(&texture, 1);
//            mat->set_uniform_texture(_mat.texture_location, 0);
////            surface.draw(
////                _vao.get(),
////                _ebo.get(),
////                mat.get(),
////                _draw_call_details[i]
////            );
//        }
//    }
//}

void SpriteBatch::add(SpriteInfo info) {
    assert(info.texture_index >= 0 && info.texture_index < _textures.size());

    ++_count;
    size_t last_index = _count - 1;
    if (_sprites.size() < _count) {
        _sprites.resize(_count);
    }
    _sprites[last_index] = info;
    _sprites_by_texture[info.texture_index].push_back(last_index);
}

void SpriteBatch::append_verts_for_sprite(const SpriteInfo &info, size_t offset) {
    float minX = info.position.x;
    float minY = info.position.y;
    float maxX = minX + info.size.x;
    float maxY = minY + info.size.y;
    Vector4 color = info.color;

    float data[COMPONENTS_PER_SPRITE] = {
        // pos,      uv,    color
        minX, minY,  0, 0,  color.x, color.y, color.z, color.w,
        maxX, minY,  1, 0,  color.x, color.y, color.z, color.w,
        minX, maxY,  0, 1,  color.x, color.y, color.z, color.w,
        maxX, maxY,  1, 1,  color.x, color.y, color.z, color.w
    };

    _vbo->set_data(offset * sizeof(float), reinterpret_cast<uint8_t *>(data), COMPONENTS_PER_SPRITE * sizeof(float));
}

void SpriteBatch::set_elements() {

    // TODO: Use a pre-allocated buffer
    size_t element_count = _sprites.size() * ELEMENTS_PER_SPRITE;
    unsigned int *data = new unsigned int[element_count];
    unsigned int *sub_buffer = data;
    IndexRange info = {0, 0};
    _draw_call_details.clear();
    for (const auto &sprite_indices : _sprites_by_texture) {

        set_elements_for_texture(sprite_indices, sub_buffer);
        info.count = sprite_indices.size() * ELEMENTS_PER_SPRITE;
        sub_buffer += info.count;
        _draw_call_details.push_back(info);
        info.offset += info.count;
    }
    _ebo->set(0, data, element_count);
}

void SpriteBatch::set_elements_for_texture(
    const vector<size_t> &sprite_indices, unsigned int *elements
) {
    for (size_t i : sprite_indices) {
        unsigned int next_index = static_cast<unsigned int>(i * VERTS_PER_SPRITE);
        elements[0] = next_index;
        elements[1] = next_index + 1;
        elements[2] = next_index + 2;
        elements[3] = next_index + 2;
        elements[4] = next_index + 1;
        elements[5] = next_index + 3;
        elements += ELEMENTS_PER_SPRITE;
    }
}
