#include <utility>
#include <cassert>
#include <algorithm>
#include "GLTexture.hpp"
#include "gl_enum_conversions.hpp"
#include "GLRenderer.hpp"
#include "operations/GenTextureGLOperation.hpp"
#include "operations/SetTextureParameterGLOperation.hpp"
#include "operations/CreateTextureStorageSyncedGLOperation.hpp"

using namespace giygas;


GLTexture::GLTexture(GLRenderer *renderer, TextureInitOptions options) {
    _renderer = renderer;
    _width = 0;
    _height = 0;

    GenTextureGLOperation gen_op;
    renderer->add_operation_and_notify(&gen_op, nullptr);
    _handle = gen_op.get_generated_handle();

    Pool<SetTextureParameterGLOperation> &pool =
        _renderer->pools().set_texture_parameter_ops;
    SetTextureParameterGLOperation *wrap_mode_u_param_op;
    SetTextureParameterGLOperation *wrap_mode_v_param_op;
    SetTextureParameterGLOperation *min_filter_param_op;
    SetTextureParameterGLOperation *mag_filter_param_op;
    wrap_mode_u_param_op = pool.take();
    wrap_mode_v_param_op = pool.take();
    min_filter_param_op = pool.take();
    mag_filter_param_op = pool.take();

    wrap_mode_u_param_op->set(
        _handle,
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        enum_for_wrap_mode(options.wrap_mode_u)
    );
    wrap_mode_v_param_op->set(
        _handle,
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        enum_for_wrap_mode(options.wrap_mode_v)
    );
    min_filter_param_op->set(
        _handle,
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        enum_for_minify_filter_mode(options.minify_filter_mode)
    );
    mag_filter_param_op->set(
        _handle,
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        enum_for_magnify_filter_mode(options.magnify_filter_mode)
    );

    renderer->add_operation(wrap_mode_u_param_op, &pool);
    renderer->add_operation(wrap_mode_v_param_op, &pool);
    renderer->add_operation(min_filter_param_op, &pool);
    renderer->add_operation(mag_filter_param_op, &pool);
}

GLTexture::GLTexture(GLTexture &&other) noexcept
    : _data(move(other._data))
{
    move_common(move(other));
}

GLTexture &GLTexture::operator=(GLTexture &&other) noexcept {
    _data = move(other._data);
    move_common(move(other));
    return *this;
}

void GLTexture::move_common(GLTexture &&other) noexcept {
    _renderer = other._renderer;
    _handle = other._handle;
    _width = other._width;
    _height = other._height;
    _data_format = other._data_format;
    other._handle = 0;
}

GLTexture::~GLTexture() {
    Pool<DeleteTextureGLOperation> &pool =
        _renderer->pools().delete_texture_ops;
    DeleteTextureGLOperation *del_op = pool.take();
    del_op->set(_handle);
    _renderer->add_operation(del_op, &pool);
}

RendererType GLTexture::renderer_type() const {
    return RendererType::OpenGL;
}

void GLTexture::set_data(
    const unsigned char* data, size_t size, size_t width, size_t height,
    TextureFormat format
) {
    size_t pixel_size = pixel_size_for_format(format);
    assert(size == width * height * pixel_size);

    size_t previous_size =
        _width * _height * pixel_size_for_format(_data_format);

    _width = width;
    _height = height;
    _data_format = format;

    if (previous_size < size) {
        // Resize buffer
        _data = unique_ptr<unsigned char[]>(new unsigned char[size]);
    }
    copy_n(data, size, _data.get());

    // TODO: Replace with map (?) research this.

    CreateTextureStorageSyncedGLOperation storage_op;
    storage_op.set(
        _handle,
        GL_TEXTURE_2D,
        0, // mip map
        GL_RGBA,
        static_cast<GLsizei>(width),
        static_cast<GLsizei>(height),
        get_gl_texture_format(format),
        GL_UNSIGNED_BYTE,
        data
    );
    _renderer->add_operation_and_notify(&storage_op, nullptr);
    storage_op.wait();
}

void GLTexture::create_storage(
    size_t width,
    size_t height,
    TextureFormat format
) {
    Pool<CreateTextureStorageGLOperation> &pool
        = _renderer->pools().create_texture_storage_ops;
    CreateTextureStorageGLOperation *storage_op = pool.take();
    storage_op->set(
        _handle,
        GL_TEXTURE_2D,
        0, // mip map,
        static_cast<GLint>(get_gl_texture_format(format)),
        static_cast<GLsizei>(width),
        static_cast<GLsizei>(height),
        GL_RGB,
        GL_UNSIGNED_BYTE,
        nullptr
    );
    _renderer->add_operation(storage_op, &pool);
}

void GLTexture::set_tex_parameter(GLenum parameter, GLint value) {
    Pool<SetTextureParameterGLOperation> &pool
        = _renderer->pools().set_texture_parameter_ops;
    SetTextureParameterGLOperation *op = pool.take();
    op->set(
        _handle,
        GL_TEXTURE_2D,
        parameter,
        value
    );
    _renderer->add_operation(op, &pool);
}

void GLTexture::set_wrap_mode_u(TextureWrapMode mode) {
    set_tex_parameter(GL_TEXTURE_WRAP_S, enum_for_wrap_mode(mode));
}

void GLTexture::set_wrap_mode_v(TextureWrapMode mode) {
    set_tex_parameter(GL_TEXTURE_WRAP_T, enum_for_wrap_mode(mode));
}

void GLTexture::set_minify_filter_mode(TextureMinifyFilterMode mode) {
    set_tex_parameter(GL_TEXTURE_MIN_FILTER, enum_for_minify_filter_mode(mode));
}

void GLTexture::set_magnify_filter_mode(TextureMagnifyFilterMode mode) {
    set_tex_parameter(GL_TEXTURE_MAG_FILTER, enum_for_magnify_filter_mode(mode));
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
