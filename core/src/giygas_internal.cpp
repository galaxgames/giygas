#include "giygas_internal.hpp"

using namespace giygas;

bool giygas::get_fallback_texture_format(TextureFormat format, TextureFormat &out) {
    if (format == TextureFormat::RGB) {
        out = TextureFormat::RGBA;
        return true;
    }
    return false;
}
