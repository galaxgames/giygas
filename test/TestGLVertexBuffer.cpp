#include "mocks/MockGL.hpp"
#include <giygas_internal/GLVertexBuffer.hpp>

using namespace giygas;
using namespace testing;

TEST(GLVertexBufferTest, TestConstructDeconstruct)
{
    MockGL gl;
    EXPECT_CALL(gl, gen_buffers(1, _)).Times(1);
    EXPECT_CALL(gl, delete_buffers(1, _)).Times(1);
    GLVertexBuffer vbo(&gl);
}

TEST(GLVertexBufferTest, TestSetData)
{
    NiceMock<MockGL> gl;
    {
        InSequence dummy;
        EXPECT_CALL(gl, bind_buffer(GL_ARRAY_BUFFER, _)).Times(1);
        EXPECT_CALL(gl, buffer_data(
            GL_ARRAY_BUFFER,
            16,
            _, // data ptr
            _  // usage hint
        )).Times(1);
    }

    GLVertexBuffer vbo(&gl);
    float data[4] = {0, 1, 2, 3};
    vbo.set_data(0, data, sizeof(data));
}
