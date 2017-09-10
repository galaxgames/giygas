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

RendererType GLTexture::renderer_type() const {
    return RendererType::OpenGL;
}

void GLTexture::set_data(
    const unsigned char* data, size_t size, size_t width, size_t height,
    TextureFormat format
) {
    _gl->bind_texture(GL_TEXTURE_2D, _handle);
    _gl->tex_image_2d(
        GL_TEXTURE_2D,
        0, // mip map
        GL_RGBA,
        static_cast<GLsizei>(width),
        static_cast<GLsizei>(height),
        0, // border, must be zero
        get_gl_texture_format(format),
        GL_UNSIGNED_BYTE,
        data
    );

    _gl->tex_parameter_i(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    _gl->tex_parameter_i(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    _gl->tex_parameter_i(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    _gl->tex_parameter_i(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//    _gl->compressed_tex_image_2d(
//        GL_TEXTURE_2D,
//        0,  // mip map
//        get_gl_texture_format(format),
//        width,
//        height,
//        0,  // border, must be zero
//        size,
//        data
//    );
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



