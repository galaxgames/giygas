#include "SetVertexAttribPointersGLOperation.hpp"

using namespace giygas;
using namespace std;

SetVertexAttribPointersGLOperation::SetVertexAttribPointersGLOperation(
) {
    //_finished = false;
}

void SetVertexAttribPointersGLOperation::set(
    GLuint vao_handle,
    GLuint vbo_handle
) {
    _vao_handle = vao_handle;
    _vbo_handle = vbo_handle;
}

void SetVertexAttribPointersGLOperation::execute(GL *gl) {
    gl->bind_vertex_array(_vao_handle);
    gl->bind_buffer(GL_ARRAY_BUFFER, _vbo_handle);

    for (VertexAttribPointerParams &params : _attrib_params) {
        gl->vertex_attrib_pointer(
            params.index,
            params.size,
            params.type,
            params.normalized,
            params.stride,
            params.pointer
        );
        gl->enable_vertex_attrib_array(params.index);
    }

//    lock_guard<mutex> lock(_mutex);
//    _finished = true;
//    _cv.notify_all();
}

//void SetVertexAttribPointersGLOperation::wait() {
//    unique_lock<mutex> lock(_mutex);
//    if (_finished) {
//        return;
//    }
//    _cv.wait(lock, [this]{ return _finished; });
//}

void SetVertexAttribPointersGLOperation::add_attrib_params(
    VertexAttribPointerParams &params
) {
    _attrib_params.push_back(params);
}
