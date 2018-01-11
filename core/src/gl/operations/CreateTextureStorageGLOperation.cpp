#include "CreateTextureStorageGLOperation.hpp"

using namespace giygas;

CreateTextureStorageGLOperation::CreateTextureStorageGLOperation() {
}

void CreateTextureStorageGLOperation::set(
    GLuint handle,
    GLenum target,
    GLint level,
    GLint internalFormat,
    GLsizei width,
    GLsizei height,
    GLenum format,
    GLenum type,
    const GLvoid *data
) {
    _handle = handle;
    _target = target;
    _level = level;
    _internalFormat = internalFormat;
    _width = width;
    _height = height;
    _format = format;
    _type = type;
    _data = data;
}

void CreateTextureStorageGLOperation::execute(GL *gl) {
    gl->bind_texture(_target, _handle);
    gl->tex_image_2d(
        _target,
        _level, // mip map
        _internalFormat,
        _width,
        _height,
        0, // border, must be zero
        _format,
        _type,
        _data
    );
}
