#pragma once
#include "../GLOperation.hpp"
#include <mutex>
#include <condition_variable>

namespace giygas {
    using namespace std;

    class MapBufferGLOperation : public GLOperation {
        GLuint _handle;
        GLenum _target;
        GLenum _access;
        void *_mapped;
        bool _finished;
        mutex _mutex;
        condition_variable _cv;
    public:
        MapBufferGLOperation(
            GLuint handle,
            GLenum target,
            GLenum access
        );
        void execute(GL *gl) override;
        void *get_mapped();
    };

}