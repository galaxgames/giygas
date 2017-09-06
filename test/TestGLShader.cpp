#include <gtest/gtest.h>
#include <giygas_internal/GLShader.hpp>
#include "mocks/MockGL.hpp"

using namespace giygas;
using namespace testing;

TEST(GLShaderTest, TestConstructDestruct)
{
    MockGL gl;
    EXPECT_CALL(gl, create_shader(GL_VERTEX_SHADER));
    EXPECT_CALL(gl, create_shader(GL_FRAGMENT_SHADER));
    EXPECT_CALL(gl, delete_shader(_)).Times(2);
    GLShader shader(&gl);
}