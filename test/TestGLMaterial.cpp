#include <gtest/gtest.h>
#include <giygas_internal/GLMaterial.hpp>
#include "mocks/MockGL.hpp"

using namespace giygas;
using namespace testing;

TEST(GLMaterialTest, TestConstructDestruct)
{
    MockGL gl;
    EXPECT_CALL(gl, create_program());
    EXPECT_CALL(gl, delete_program(_));

    GLMaterial material(&gl);
}

TEST(GLMaterialTest, TestSetUniformFloat)
{
    

}