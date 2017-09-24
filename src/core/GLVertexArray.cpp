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
    const VertexBuffer *buffer, const VertexAttributeLayout &layout
) {
    assert(buffer->get_renderer_type() == RendererType::OpenGL);
    const auto *glbuffer = reinterpret_cast<const GLVertexBuffer *>(buffer);

    const auto& attributes = layout.attributes();

    _gl->bind_vertex_array(_handle);
    _gl->bind_buffer(GL_ARRAY_BUFFER, glbuffer->get_handle());

    for (const LayoutAttribute& attrib : attributes) {
        if (attrib.component_count == 0 || attrib.component_size == 0) {
            ++_next_index;
            continue;
        }

        GLenum type;

        switch (attrib.component_size) {
        case 2:
            type = GL_HALF_FLOAT;
            break;
        case 4:
            type = GL_FLOAT;
            break;
        case 8:
            type = GL_DOUBLE;
            break;
        default:
            assert(false);
            break;
        }

        _gl->vertex_attrib_pointer(
            _next_index,                                    // index
            static_cast<GLint>(attrib.component_count),     // size
            type,                                           // type
            GL_FALSE,                                       // normalized
            static_cast<GLsizei>(layout.stride),            // stride
            reinterpret_cast<GLvoid *>(attrib.offset)       // pointer
        );

        _gl->enable_vertex_attrib_array(_next_index);
        ++_next_index;
    }
}

GLuint GLVertexArray::get_handle() const {
    return _handle;
}
