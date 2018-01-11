#pragma once
#include <giygas/Vector2.hpp>
#include <giygas/Vector3.hpp>
#include <giygas/Vector4.hpp>
#include <giygas/Matrix4x4.hpp>
#include "GL.hpp"

#define UNIFORM_VALUE_CLASS(class_name, T) \
    class class_name : public GLUniformValue { \
        T _value; \
    public: \
        class_name(T value); \
        void do_gl_call(GL *gl, GLint location) override; \
    }


namespace giygas {

    class GLUniformValue {
    public:
        virtual ~GLUniformValue() = default;
        virtual void do_gl_call(GL *gl, GLint location) = 0;
    };

    UNIFORM_VALUE_CLASS(FloatUniformValue, float);
    UNIFORM_VALUE_CLASS(IntUniformValue, int);
    UNIFORM_VALUE_CLASS(Vector2UniformValue, Vector2);
    UNIFORM_VALUE_CLASS(Vector3UniformValue, Vector3);
    UNIFORM_VALUE_CLASS(Vector4UniformValue, Vector4);

    class Matrix4x4UniformValue : public GLUniformValue {
        Matrix4x4 _value;
    public:
        Matrix4x4UniformValue(const Matrix4x4 &value);
        void do_gl_call(GL *gl, GLint location) override;
    };

}
