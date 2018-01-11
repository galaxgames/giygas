#include "AttachTextureToFramebufferGLOperation.hpp"

using namespace giygas;

void AttachTextureToFramebufferGLOperation::set(
    GLuint framebuffer,
    GLuint texture,
    GLint level,
    GLenum attachment_type
) {
    _framebuffer = framebuffer;
    _texture = texture;
    _level = level;
    _attachment_type = attachment_type;
}

void AttachTextureToFramebufferGLOperation::execute(GL *gl) {
    gl->bind_framebuffer(GL_FRAMEBUFFER, _framebuffer);
    gl->framebuffer_texture_2d(
        GL_FRAMEBUFFER,
        _attachment_type,
        GL_TEXTURE_2D,
        _texture,
        _level
    );
}