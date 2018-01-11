#include "GLUniformValue.hpp"


#define UNIFORM_VALUE_IMPL(class_name, T, impl) \
    class_name::class_name(T value) \
        : _value(value) \
    { \
    } \
    \
    void class_name::do_gl_call(GL *gl, GLint location) impl


using namespace giygas;


UNIFORM_VALUE_IMPL(FloatUniformValue, float, {
gl->uniform_1f(location, _value);
})

UNIFORM_VALUE_IMPL(IntUniformValue, int, {
gl->uniform_1i(location, _value);
})

UNIFORM_VALUE_IMPL(Vector2UniformValue, Vector2, {
gl->uniform_2f(location, _value.x, _value.y);
})

UNIFORM_VALUE_IMPL(Vector3UniformValue, Vector3, {
gl->uniform_3f(location, _value.x, _value.y, _value.z);
})

UNIFORM_VALUE_IMPL(Vector4UniformValue, Vector4, {
gl->uniform_4f(location, _value.x, _value.y, _value.z, _value.w);
})

Matrix4x4UniformValue::Matrix4x4UniformValue(const Matrix4x4 &value)
    :_value(value)
{
}

void Matrix4x4UniformValue::do_gl_call(GL *gl, GLint location) {
    gl->uniform_matrix_4fv(
        location, 1, false, reinterpret_cast<float *>(&_value)
    );
}