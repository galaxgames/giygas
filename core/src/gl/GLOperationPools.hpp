#pragma once
#include "Pool.hpp"
#include "operations/GenTextureGLOperation.hpp"
#include "operations/BufferDataGLOperation.hpp"
#include "operations/DeleteBufferGLOperation.hpp"
#include "operations/BufferSubDataGLOperation.hpp"
#include "operations/SetTextureParameterGLOperation.hpp"
#include "operations/DeleteTextureGLOperation.hpp"
#include "operations/CreateTextureStorageGLOperation.hpp"
#include "operations/CreateShaderGLOperation.hpp"
#include "operations/DeleteShaderGLOperation.hpp"
#include "operations/DeleteVertexArrayGLOperation.hpp"
#include "operations/DeleteProgramGLOperation.hpp"
#include "operations/SetVertexAttribPointersGLOperation.hpp"
#include "operations/SetUniformValueGLOperation.hpp"
#include "operations/DeleteFramebufferGLOperation.hpp"
#include "operations/AttachTextureToFramebufferGLOperation.hpp"
#include "operations/AttachRenderbufferToFramebufferGLOperation.hpp"
#include "operations/RenderGLOperation.hpp"
#include "operations/ClearGLOperation.hpp"
#include "operations/DeleteRenderbufferGLOperation.hpp"
#include "operations/CreateRenderbufferStorageGLOperation.hpp"
#include "operations/Pipeline/SetPipelineGLOperation.hpp"
#include "operations/CompileShaderGLOperation.hpp"

namespace giygas {
    class GLOperationPools {
    public:

        // Pipeline Operations
        Pool<SetPipelineGLOperation> _set_pipeline_ops;

        // Buffer Operations
        Pool<DeleteBufferGLOperation> delete_buffer_ops;
        Pool<BufferDataGLOperation> buffer_data_ops;
        Pool<BufferSubDataGLOperation> buffer_sub_data_ops;

        // Vertex Array Operations
        Pool<DeleteVertexArrayGLOperation> delete_vertex_array_ops;
        Pool<SetVertexAttribPointersGLOperation> set_vertex_attrib_pointers_ops;

        // Texture Operations
        Pool<DeleteTextureGLOperation> delete_texture_ops;
        Pool<SetTextureParameterGLOperation> set_texture_parameter_ops;
        Pool<CreateTextureStorageGLOperation> create_texture_storage_ops;

        // Shader Operations
        Pool<CompileShaderGLOperation> compile_shader_ops;
        Pool<DeleteShaderGLOperation> delete_shader_ops;

        // Program Operations
        Pool<DeleteProgramGLOperation> delete_program_ops;
        Pool<SetUniformValueGLOperation> set_uniform_value_ops;

        // Framebuffer Operations
        Pool<DeleteFramebufferGLOperation> delete_framebuffer_ops;
        Pool<AttachTextureToFramebufferGLOperation>
            attach_texture_to_framebuffer_ops;
        Pool<AttachRenderbufferToFramebufferGLOperation>
            attach_renderbuffer_to_framebuffer_ops;

        // Renderbuffer Operations
        Pool<DeleteRenderbufferGLOperation> delete_renderbuffer_ops;
        Pool<CreateRenderbufferStorageGLOperation>
            create_renderbuffer_storage_ops;

        // Render Operations
        Pool<RenderGLOperation> render_ops;
        Pool<ClearGLOperation> clear_ops;

        GLOperationPools &operator=(const GLOperationPools &other) = delete;
    };

    extern template class Pool<SetPipelineGLOperation>;

    extern template class Pool<DeleteBufferGLOperation>;
    extern template class Pool<BufferDataGLOperation>;
    extern template class Pool<BufferSubDataGLOperation>;

    extern template class Pool<DeleteVertexArrayGLOperation>;
    extern template class Pool<SetVertexAttribPointersGLOperation>;

    extern template class Pool<DeleteTextureGLOperation>;
    extern template class Pool<SetTextureParameterGLOperation>;
    extern template class Pool<CreateTextureStorageGLOperation>;

    extern template class Pool<DeleteShaderGLOperation>;

    extern template class Pool<DeleteProgramGLOperation>;
    extern template class Pool<SetUniformValueGLOperation>;

    extern template class Pool<DeleteFramebufferGLOperation>;
    extern template class Pool<AttachTextureToFramebufferGLOperation>;
    extern template class Pool<AttachRenderbufferToFramebufferGLOperation>;

    extern template class Pool<DeleteRenderbufferGLOperation>;
    extern template class Pool<CreateRenderbufferStorageGLOperation>;

    extern template class Pool<RenderGLOperation>;
    extern template class Pool<ClearGLOperation>;
}