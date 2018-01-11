#include "GLVertexArray.hpp"
#include "GLVertexBuffer.hpp"
#include "operations/GenVertexArrayGLOperation.hpp"
#include "operations/DeleteVertexArrayGLOperation.hpp"
#include "operations/SetVertexAttribPointersGLOperation.hpp"
#include <cassert>

using namespace giygas;
using namespace std;


GLVertexArray::GLVertexArray(GLRenderer *renderer) {
    _renderer = renderer;
    _next_index = 0;

    GenVertexArrayGLOperation gen_op;
    _renderer->add_operation(&gen_op, nullptr);
    _handle = gen_op.get_generated_handle();
}

GLVertexArray::GLVertexArray(GLVertexArray &&other) noexcept {
    *this = move(other);
}

GLVertexArray &GLVertexArray::operator=(GLVertexArray &&other) noexcept {
    _renderer = other._renderer;
    _handle = other._handle;
    _next_index = other._next_index;
    other._handle = 0;

    return *this;
}

GLVertexArray::~GLVertexArray() {
    Pool<DeleteVertexArrayGLOperation> &pool
        = _renderer->pools().delete_vertex_array_ops;
    DeleteVertexArrayGLOperation *op = pool.take();
    op->set(_handle);
    _renderer->add_operation(op, &pool);
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

    Pool<SetVertexAttribPointersGLOperation> &pool
        = _renderer->pools().set_vertex_attrib_pointers_ops;
    SetVertexAttribPointersGLOperation *op = pool.take();
    op->set(_handle, glbuffer->get_handle());

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

        VertexAttribPointerParams params;
        params.index = _next_index;
        params.size = static_cast<GLint>(attrib.component_count);
        params.type = type;
        params.normalized = GL_FALSE;
        params.stride = static_cast<GLsizei>(layout.stride);
        params.pointer = reinterpret_cast<GLvoid *>(attrib.offset);

        ++_next_index;

        op->add_attrib_params(params);
    }

    _renderer->add_operation(op, &pool);
}

GLuint GLVertexArray::get_handle() const {
    return _handle;
}
