#pragma once
#include "../GLOperation.hpp"

namespace giygas {

    class AttachRenderbufferToFramebufferGLOperation : public GLOperation {
        GLuint _framebuffer;
        GLuint _texture;
        GLenum _attachment_type;
    public:
        void set(
            GLuint framebuffer,
            GLuint renderbuffer,
            GLenum attachment_type
        );
        void execute(GL *gl) override;
    };

}