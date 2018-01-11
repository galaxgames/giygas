#pragma once
#include "../GLOperation.hpp"

namespace giygas {

    class ClearGLOperation : public GLOperation {
        GLuint _framebuffer;
        GLbitfield _mask;
        GLfloat _color_r;
        GLfloat _color_g;
        GLfloat _color_b;
        GLfloat _color_a;
        GLdouble _depth;
        GLint _stencil;
    public:
        void set(GLuint framebuffer, GLbitfield mask);
        void set_color(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
        void set_depth(GLdouble depth);
        void set_stencil(GLint stencil);
        void execute(GL *gl) override;
    };

}