#pragma once
#include "../GLOperation.hpp"

namespace giygas {

    class AttachTextureToFramebufferGLOperation : public GLOperation {
        GLuint _framebuffer;
        GLuint _texture;
        GLint _level;
        GLenum _attachment_type;
    public:
        void set(
            GLuint framebuffer,
            GLuint texture,
            GLint level,
            GLenum attachment_type
        );
        void execute(GL *gl) override;
    };

}