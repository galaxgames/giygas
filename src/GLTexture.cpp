#include <utility>
#include "giygas_internal/GLTexture.hpp"

using namespace giygas;

GLTexture::GLTexture(GL *gl) {
    _gl = gl;
    gl->gen_textures(1, &_handle);
}

GLTexture::GLTexture(GLTexture &&other) noexcept {
    *this = std::move(other);
}

GLTexture &GLTexture::operator=(GLTexture &&other) noexcept {
    _gl = other._gl;
    _handle = other._handle;
    other._handle = 0;

    return *this;
}

GLTexture::~GLTexture() {
    _gl->delete_textures(1, &_handle);
}

void GLTexture::set_data(
    const char* data, int size, int width, int height, TextureFormat format
) {
    _gl->bind_texture(GL_TEXTURE_2D, _handle);
    _gl->compressed_tex_image_2d(
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
    }
}



