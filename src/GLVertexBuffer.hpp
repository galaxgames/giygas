#pragma once
#include <glad/glad.h>
#include <giygas/VertexBuffer.hpp>

namespace giygas {
    class GLVertexBuffer : public VertexBuffer {
        // handle to the GL buffer
        GLuint _handle;

        // handle to the GL vertex array
        GLuint _vertex_array;

        // The internal buffer
        float* _data;

        // Count of single vertex components
        size_t _length;

        GLuint _channel_count;

        void set_data(int offset, const float *data, int count);

    public:
        GLVertexBuffer();
        GLVertexBuffer(const GLVertexBuffer&) = delete;
        GLVertexBuffer& operator=(const GLVertexBuffer&) = delete;
        GLVertexBuffer(GLVertexBuffer&&) noexcept;
        virtual ~GLVertexBuffer();

        void set_layout(const VertexBufferLayout &layout) override;
        bool has_layout() const override;
        void set_vector2_channel(int offset, const Vector2 *verts, int count) override;
        void set_vector3_channel(int offset, const Vector3 *verts, int count) override;
        void set_vector4_channel(int offset, const Vector4 *verts, int count) override;

        GLuint get_handle() const;
    };
}