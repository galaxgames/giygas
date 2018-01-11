#include "gl_enum_conversions.hpp"

using namespace giygas;

GLenum giygas::get_gl_texture_format(TextureFormat format) {
    switch (format) {
    case TextureFormat::RGB:
        return GL_RGB;
    case TextureFormat::RGBA:
        return GL_RGBA;
    case TextureFormat::Depth16:
        return GL_DEPTH_COMPONENT16;
    case TextureFormat::Depth24:
        return GL_DEPTH_COMPONENT24;
    case TextureFormat::Depth32:
        return GL_DEPTH_COMPONENT32;
    case TextureFormat::Depth32Float:
        return GL_DEPTH_COMPONENT32F;
    }
}
