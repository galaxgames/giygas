#include <utility>
#include <cassert>
#include "giygas_internal/GLTexture.hpp"
#include <giygas_internal/gl_enum_conversions.hpp>

using namespace giygas;

GLTexture::GLTexture(GL *gl, TextureInitOptions options) {
    _gl = gl;
    gl->gen_textures(1, &_handle);
    gl->bind_texture(GL_TEXTURE_2D, _handle);

    _gl->tex_parameter_i(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        enum_for_wrap_mode(options.wrap_mode_u)
    );
    _gl->tex_parameter_i(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        enum_for_wrap_mode(options.wrap_mode_v)
    );
    _gl->tex_parameter_i(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        enum_for_minify_filter_mode(options.minify_filter_mode)
    );
    _gl->tex_parameter_i(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        enum_for_magnify_filter_mode(options.magnify_filter_mode)
    );
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
    assert(size == width * height * pixel_size_for_format(format));

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
}

void GLTexture::create_storage(
    size_t width,
    size_t height,
    TextureFormat format
) {
    _gl->bind_texture(GL_TEXTURE_2D, _handle);
    _gl->tex_image_2d(
        GL_TEXTURE_2D,
        0, // mip map,
        static_cast<GLint>(get_gl_texture_format(format)),
        static_cast<GLsizei>(width),
        static_cast<GLsizei>(height),
        0, // border, must be zero,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        nullptr
    );
}

void GLTexture::set_wrap_mode_u(TextureWrapMode mode) {
    _gl->bind_texture(GL_TEXTURE_2D, _handle);
    _gl->tex_parameter_i(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        enum_for_wrap_mode(mode)
    );
}

void GLTexture::set_wrap_mode_v(TextureWrapMode mode) {
    _gl->bind_texture(GL_TEXTURE_2D, _handle);
    _gl->tex_parameter_i(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        enum_for_wrap_mode(mode)
    );
}

void GLTexture::set_minify_filter_mode(TextureMinifyFilterMode mode) {
    _gl->bind_texture(GL_TEXTURE_2D, _handle);
    _gl->tex_parameter_i(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        enum_for_minify_filter_mode(mode)
    );
}

void GLTexture::set_magnify_filter_mode(TextureMagnifyFilterMode mode) {
    _gl->bind_texture(GL_TEXTURE_2D, _handle);
    _gl->tex_parameter_i(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        enum_for_magnify_filter_mode(mode)
    );
}

GLuint GLTexture::handle() const {
    return _handle;
}

size_t GLTexture::pixel_size_for_format(TextureFormat format) {
    switch (format) {
    case TextureFormat::Depth16:
        return 2;
    case TextureFormat::RGB:
    case TextureFormat::Depth24:
        return 3;
    case TextureFormat::RGBA:
    case TextureFormat::Depth32:
    case TextureFormat::Depth32Float:
        return 4;
    }
}

GLenum GLTexture::enum_for_wrap_mode(TextureWrapMode mode) {
    switch (mode) {
    case TextureWrapMode::ClampToEdge:
        return GL_CLAMP_TO_EDGE;
    case TextureWrapMode::ClampToBorder:
        return GL_CLAMP_TO_BORDER;
    case TextureWrapMode ::MirroredRepeat:
        return GL_MIRRORED_REPEAT;
    case TextureWrapMode ::Repeat:
        return GL_REPEAT;
    }
}

GLenum GLTexture::enum_for_minify_filter_mode(TextureMinifyFilterMode mode) {
    switch(mode) {
    case TextureMinifyFilterMode::Linear:
        return GL_LINEAR;
    case TextureMinifyFilterMode::Nearest:
        return GL_NEAREST;
    case TextureMinifyFilterMode::LinearMipMapLinear:
        return GL_LINEAR_MIPMAP_LINEAR;
    case TextureMinifyFilterMode::LinearMipMapNearest:
        return GL_LINEAR_MIPMAP_NEAREST;
    case TextureMinifyFilterMode::NearestMipMapLinear:
        return GL_NEAREST_MIPMAP_LINEAR;
    case TextureMinifyFilterMode::NearestMipMapNearest:
        return GL_NEAREST_MIPMAP_NEAREST;
    }
}

GLenum GLTexture::enum_for_magnify_filter_mode(TextureMagnifyFilterMode mode) {
    switch(mode) {
        case TextureMagnifyFilterMode::Linear:
            return GL_LINEAR;
        case TextureMagnifyFilterMode::Nearest:
            return GL_NEAREST;
    }
}
