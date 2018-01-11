#include "RenderGLOperation.hpp"

using namespace giygas;

void RenderGLOperation::set(
    GLuint framebuffer,
    GLuint vertex_array,
    GLuint element_buffer,
    GLuint program
) {
    _framebuffer = framebuffer;
    _vertex_array = vertex_array;
    _element_buffer = element_buffer;
    _program = program;
}

void RenderGLOperation::set_viewport(
    GLint viewport_x,
    GLint viewport_y,
    GLsizei viewport_width,
    GLsizei viewport_height
) {
    _viewport_x = viewport_x;
    _viewport_y = viewport_y;
    _viewport_width = viewport_width;
    _viewport_height = viewport_height;
}

void RenderGLOperation::clear_textures() {
    _textures.clear();
}

void RenderGLOperation::add_texture(GLuint texture) {
    _textures.push_back(texture);
}

void RenderGLOperation::set_primitive(GLenum primitive) {
    _primitive = primitive;
}

void RenderGLOperation::set_element_type(GLenum type) {
    _element_type = type;
}

void RenderGLOperation::set_draw_info(ElementDrawInfo info) {
    _draw_info = info;
}

void RenderGLOperation::execute(GL *gl) {
    gl->bind_framebuffer(GL_FRAMEBUFFER, _framebuffer);

    // TODO: Keep track of last bound framebuffer and only call this if
    // last bound framebuffer wasn't ours and viewport values are not dirty.
    gl->viewport(
        _viewport_x,
        _viewport_y,
        _viewport_width,
        _viewport_height
    );

    gl->bind_vertex_array(_vertex_array);
    gl->bind_buffer(GL_ELEMENT_ARRAY_BUFFER, _element_buffer);
    gl->use_program(_program);

    for (size_t i = 0, ilen = _textures.size(); i < ilen; ++i) {
        gl->active_texture(static_cast<GLenum>(GL_TEXTURE0 + i));
        gl->bind_texture(GL_TEXTURE_2D, _textures[i]);
    }

    gl->draw_elements(
        _primitive,
        static_cast<GLsizei>(_draw_info.count),
        _element_type,
        (GLvoid *)_draw_info.offset
    );
}