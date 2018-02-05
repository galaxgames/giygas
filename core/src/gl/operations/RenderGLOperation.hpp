#pragma once
#include "../GLOperation.hpp"
#include <vector>
#include <giygas/IndexRange.hpp>

namespace giygas {
    using namespace std;

    class RenderGLOperation : public GLOperation {
        GLuint _framebuffer;
        GLuint _vertex_array;
        GLuint _element_buffer;
        GLuint _program;

        GLint _viewport_x;
        GLint _viewport_y;
        GLsizei _viewport_width;
        GLsizei _viewport_height;

        GLenum _primitive;
        GLenum _element_type;
        ElementDrawInfo _draw_info;

        vector<GLuint> _textures;

    public:
        void set(
            GLuint framebuffer,
            GLuint vertex_array,
            GLuint element_buffer,
            GLuint program
        );
        void set_viewport(
            GLint viewport_x,
            GLint viewport_y,
            GLsizei viewport_width,
            GLsizei viewport_height
        );
        void clear_textures();
        void add_texture(GLuint texture);
        void set_primitive(GLenum primitive);
        void set_element_type(GLenum type);
        void set_draw_info(ElementDrawInfo info);
        void execute(GL *gl) override;
    };

}