//#include <cassert>
//#include "VulkanMaterial.hpp"
//#include "VulkanRenderer.hpp"
//
//using namespace giygas;
//
//VulkanMaterial::VulkanMaterial(VulkanRenderer *renderer) {
//    _renderer = renderer;
//}
//
//VulkanMaterial::~VulkanMaterial() {
//}
//
//RendererType VulkanMaterial::renderer_type() const {
//    return RendererType::Vulkan;
//}
//
//void VulkanMaterial::set_textures(
//    const Texture **textures,
//    const Sampler **samplers,
//    size_t count
//) {
//    _textures = unique_ptr<VulkanMaterial::TextureData[]>(
//        new VulkanMaterial::TextureData[count]
//    );
//    for (size_t i = 0; i < count; ++i) {
//        _textures[i].texture = textures[i];
//        _textures[i].sampler = samplers[i];
//    }
//    _texture_count = count;
//}
//
//void VulkanMaterial::reset(size_t size) {
//    _data = unique_ptr<uint8_t[]>(new uint8_t[size]);
//    _data_size = size;
//}
//
//void VulkanMaterial::set_uniform_float(size_t offset, float value) {
//    assert(offset + sizeof(float) < _data_size);
//    auto *ptr = reinterpret_cast<float *>(_data.get() + offset);
//    *ptr = value;
//}
//
//void VulkanMaterial::set_uniform_texture(size_t offset, uint32_t value) {
//    assert(offset + sizeof(uint32_t) < _data_size);
//    auto *ptr = reinterpret_cast<uint32_t *>(_data.get() + offset);
//}
//
//void VulkanMaterial::set_uniform_vector2(size_t offset, Vector2 value) {
//    assert(offset + sizeof(float) * 2 < _data_size);
//    auto *ptr = reinterpret_cast<float *>(_data.get() + offset);
//    ptr[0] = value.x;
//    ptr[1] = value.y;
//}
//
//void VulkanMaterial::set_uniform_vector3(size_t offset, Vector3 value) {
//    assert(offset + sizeof(float) * 4 < _data_size);
//    auto *ptr = reinterpret_cast<float *>(_data.get() + offset);
//    ptr[0] = value.x;
//    ptr[1] = value.y;
//    ptr[2] = value.z;
//    // We don't care about the 4th element. (vector 3s are padded out to 4 components)
//}
//
//void VulkanMaterial::set_uniform_vector4(size_t offset, Vector4 value) {
//    assert(offset + sizeof(float) * 4 < _data_size);
//    auto *ptr = reinterpret_cast<float *>(_data.get() + offset);
//    ptr[0] = value.x;
//    ptr[1] = value.y;
//    ptr[2] = value.z;
//    ptr[3] = value.w;
//}
//
//void VulkanMaterial::set_uniform_matrix4x4(size_t offset, const Matrix4x4 &value) {
//    assert(offset + sizeof(float) * 16 < _data_size);
//    auto *ptr = reinterpret_cast<float *>(_data.get() + offset);
//    ptr[0] = value.c0r0;
//    ptr[1] = value.c0r1;
//    ptr[2] = value.c0r2;
//    ptr[3] = value.c0r3;
//    ptr[4] = value.c1r0;
//    ptr[5] = value.c1r1;
//    ptr[6] = value.c1r2;
//    ptr[7] = value.c1r3;
//    ptr[8] = value.c2r0;
//    ptr[9] = value.c2r1;
//    ptr[10] = value.c2r2;
//    ptr[11] = value.c2r3;
//    ptr[12] = value.c3r0;
//    ptr[13] = value.c3r1;
//    ptr[14] = value.c3r2;
//    ptr[15] = value.c3r3;
//}
