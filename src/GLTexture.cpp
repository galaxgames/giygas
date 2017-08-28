#include "GLTexture.hpp"

using namespace giygas;

GLTexture::GLTexture() {
    glGenTextures(1, &_handle);
}

GLTexture::GLTexture(GLTexture &&other) {
    _handle = other._handle;
    other._handle = 0;
}

GLTexture::~GLTexture() {
    glDeleteTextures(1, &_handle);
}

void GLTexture::set_data(
    const char* data, int size, int width, int height, TextureFormat format
) {
    glBindTexture(GL_TEXTURE_2D, _handle);
    glCompressedTexImage2D(
        GL_TEXTURE_2D,
        0,  // mip map
        get_gl_texture_format(format),
        width,
        height,
        0,  // border, must be zero
        size,
        data
    );
}

GLuint GLTexture::get_handle() const {
    return _handle;
}

GLenum GLTexture::get_gl_texture_format(TextureFormat format) {
    switch (format) {
    case TextureFormat::RGB:
        return GL_RGB;
    case TextureFormat::RGBA:
        return GL_RGBA;
    default:
        return GL_NONE;
    }
}



