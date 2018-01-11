#pragma once
#include "../GLOperation.hpp"

namespace giygas {

    class CreateRenderbufferStorageGLOperation : public GLOperation {
        GLuint _handle;
        GLint _internalFormat;
        GLsizei _width;
        GLsizei _height;
    public:
        CreateRenderbufferStorageGLOperation();
        void set(
            GLuint handle,
            GLint internalFormat,
            GLsizei width,
            GLsizei height
        );
        void execute(GL *gl) override;
    };
}