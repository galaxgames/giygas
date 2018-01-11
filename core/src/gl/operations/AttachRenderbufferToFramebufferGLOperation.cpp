#include "AttachRenderbufferToFramebufferGLOperation.hpp"

using namespace giygas;

void AttachRenderbufferToFramebufferGLOperation::set(
    GLuint framebuffer,
    GLuint renderbuffer,
    GLenum attachment_type
) {
    _framebuffer = framebuffer;
    _texture = renderbuffer;
    _attachment_type = attachment_type;
}

void AttachRenderbufferToFramebufferGLOperation::execute(GL *gl) {
    gl->bind_framebuffer(GL_FRAMEBUFFER, _framebuffer);
    gl->framebuffer_renderbuffer(
        GL_FRAMEBUFFER,
        _attachment_type,
        GL_RENDERBUFFER,
        _texture
    );
}