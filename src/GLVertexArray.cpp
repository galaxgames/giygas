#include <giygas_internal/GLVertexArray.hpp>
#include <giygas_internal/GLVertexBuffer.hpp>
#include <cassert>

using namespace giygas;
using namespace std;


GLVertexArray::GLVertexArray(GL *gl) {
    _gl = gl;
    _next_index = 0;
    _gl->gen_vertex_arrays(1, &_handle);
}

GLVertexArray::GLVertexArray(GLVertexArray &&other) noexcept {
    *this = move(other);
}

GLVertexArray &GLVertexArray::operator=(GLVertexArray &&other) noexcept {
    _gl = other._gl;
    _handle = other._handle;
    _next_index = other._next_index;
    other._handle = 0;

    return *this;
}

GLVertexArray::~GLVertexArray() {
    _gl->delete_vertex_arrays(1, &_handle);
}

RendererType GLVertexArray::get_renderer_type() const {
    return RendererType::OpenGL;
}

void GLVertexArray::add_buffer(
    const VertexBuffer *buffer, const VertexBufferLayout &layout
) {
    assert(buffer->get_renderer_type() == RendererType::OpenGL);
    const auto *glbuffer = reinterpret_cast<const GLVertexBuffer *>(buffer);

    const auto& channels = layout.get_channels();

    int stride = 0;
    for (int size : channels) {
        stride += size;
    }

    int offset = 0;

    _gl->bind_vertex_array(_handle);
    _gl->bind_buffer(GL_ARRAY_BUFFER, glbuffer->get_handle());

    for (int size : channels) {
        _gl->vertex_attrib_pointer(
            _next_index,                             // index
            size,                              // size
            GL_FLOAT,                          // type
            GL_FALSE,                          // normalized
            static_cast<GLsizei>(stride) * static_cast<GLsizei>(sizeof(GLfloat)),                     // stride
            reinterpret_cast<GLvoid *>(offset) // pointer
        );

        _gl->enable_vertex_attrib_array(_next_index);

        offset += size * sizeof(GLfloat);
        ++_next_index;
    }
}

GLuint GLVertexArray::get_handle() const {
    return _handle;
}
