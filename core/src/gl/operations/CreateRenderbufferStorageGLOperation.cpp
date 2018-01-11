#include "CreateRenderbufferStorageGLOperation.hpp"

using namespace giygas;

CreateRenderbufferStorageGLOperation::CreateRenderbufferStorageGLOperation() {
}

void CreateRenderbufferStorageGLOperation::set(
    GLuint handle,
    GLint internalFormat,
    GLsizei width,
    GLsizei height
) {
    _handle = handle;
    _internalFormat = internalFormat;
    _width = width;
    _height = height;
}

void CreateRenderbufferStorageGLOperation::execute(GL *gl) {
    gl->bind_renderbuffer(GL_RENDERBUFFER, _handle);
    gl->renderbuffer_storage(
        GL_RENDERBUFFER,
        _internalFormat,
        _width,
        _height
    );
}
