#include "mocks/MockGL.hpp"
#include <giygas_internal/GLVertexBuffer.hpp>

using namespace giygas;
using namespace testing;

TEST(GLVertexBufferTest, TestConstructDeconstruct)
{
    MockGL gl;
    EXPECT_CALL(gl, gen_buffers(1, _)).Times(1);
    EXPECT_CALL(gl, gen_vertex_arrays(1, _)).Times(1);
    EXPECT_CALL(gl, delete_buffers(1, _)).Times(1);
    EXPECT_CALL(gl, delete_vertex_arrays(1, _)).Times(1);
    GLVertexBuffer vbo(&gl);
}



