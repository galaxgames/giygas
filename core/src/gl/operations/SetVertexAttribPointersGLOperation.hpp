#pragma once
#include <mutex>
#include <condition_variable>
#include <vector>
#include "../GLOperation.hpp"

namespace giygas {
    using namespace std;

    class VertexAttribPointerParams {
    public:
        GLuint index;
        GLint size;
        GLenum type;
        GLboolean normalized;
        GLsizei stride;
        const GLvoid *pointer;
    };


    class SetVertexAttribPointersGLOperation : public GLOperation {
        GLuint _vao_handle;
        GLuint _vbo_handle;
        vector<VertexAttribPointerParams> _attrib_params;
//        bool _finished;
//        mutex _mutex;
//        condition_variable _cv;
    public:
        SetVertexAttribPointersGLOperation();
        void set(GLuint vao_handle, GLuint vbo_handle);
        void execute(GL *gl) override;
        //void wait();

        // TODO: Make an emplace version of this function?
        void add_attrib_params(VertexAttribPointerParams &params);
    };

}