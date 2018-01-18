#pragma once
#include <mutex>
#include <condition_variable>
#include "CreateTextureStorageGLOperation.hpp"

namespace giygas {
    using namespace std;

    class CreateTextureStorageSyncedGLOperation : public GLOperation {
        CreateTextureStorageGLOperation _base;
        bool _finished;
        mutex _mutex;
        condition_variable _cv;
    public:
        CreateTextureStorageSyncedGLOperation();
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
        void wait();
    };

}