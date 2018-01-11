#pragma once
#include "../GLOperation.hpp"

namespace giygas {

    class CreateTextureStorageGLOperation : public GLOperation {
        GLuint _handle;
        GLenum _target;
        GLint _level;
        GLint _internalFormat;
        GLsizei _width;
        GLsizei _height;
        GLenum _format;
        GLenum _type;
        const GLvoid *_data;
    public:
        CreateTextureStorageGLOperation();
        void set(
            GLuint handle,
            GLenum target,
            GLint level,
            GLint internalFormat,
            GLsizei width,
            GLsizei height,
            GLenum format,
            GLenum type,
            const GLvoid *data
        );
        void execute(GL *gl) override;
    };
}