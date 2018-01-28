#include "GLOperationPools.hpp"


// ***
#include "Pool.cpp"
// ***


namespace giygas {
    template class Pool<SetPipelineGLOperation>;

    template class Pool<DeleteBufferGLOperation>;
    template class Pool<BufferDataGLOperation>;
    template class Pool<BufferSubDataGLOperation>;

    template class Pool<DeleteVertexArrayGLOperation>;
    template class Pool<SetVertexAttribPointersGLOperation>;

    template class Pool<DeleteTextureGLOperation>;
    template class Pool<SetTextureParameterGLOperation>;
    template class Pool<CreateTextureStorageGLOperation>;

    template class Pool<CompileShaderGLOperation>;
    template class Pool<DeleteShaderGLOperation>;

    template class Pool<DeleteProgramGLOperation>;
    template class Pool<SetUniformValueGLOperation>;

    template class Pool<DeleteFramebufferGLOperation>;
    template class Pool<AttachTextureToFramebufferGLOperation>;
    template class Pool<AttachRenderbufferToFramebufferGLOperation>;

    template class Pool<DeleteRenderbufferGLOperation>;
    template class Pool<CreateRenderbufferStorageGLOperation>;

    template class Pool<RenderGLOperation>;
    template class Pool<ClearGLOperation>;

}