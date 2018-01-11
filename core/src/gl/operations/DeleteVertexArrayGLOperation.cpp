#include "DeleteVertexArrayGLOperation.hpp"

using namespace giygas;
using namespace std;

DeleteVertexArrayGLOperation::DeleteVertexArrayGLOperation() {
}

void DeleteVertexArrayGLOperation::set(GLuint handle) {
    _handle = handle;
}

void DeleteVertexArrayGLOperation::execute(GL *gl) {
    gl->delete_vertex_arrays(1, &_handle);
//    lock_guard<mutex> lock(_mutex);
//    _handle = 0;
//    _cv.notify_all();
}

//void DeleteVertexArrayGLOperation::wait() {
//    unique_lock<mutex> lock(_mutex);
//    if (_handle == 0) {
//        return;
//    }
//    _cv.wait(lock, [this]{ return _handle == 0; });
//}
