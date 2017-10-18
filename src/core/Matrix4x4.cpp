#include <giygas/Matrix4x4.hpp>
#include <cmath>
#include <giygas_internal/math.hpp>

using namespace giygas;
using namespace std;

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &rhs) const {
    Matrix4x4 lhs = *this;
    return lhs *= rhs;
}

Matrix4x4 &Matrix4x4::operator*=(const Matrix4x4 &rhs) {
    // Note: Parenthesis added for readability.
    // TODO: Look for optimizations here.
    c0r0 = (c0r0 * rhs.c0r0) + (c1r0 * rhs.c0r1) + (c2r0 * rhs.c0r2) + (c3r0 * rhs.c0r3);
    c0r1 = (c0r1 * rhs.c0r0) + (c1r1 * rhs.c0r1) + (c2r1 * rhs.c0r2) + (c3r1 * rhs.c0r3);
    c0r2 = (c0r2 * rhs.c0r0) + (c1r2 * rhs.c0r1) + (c2r2 * rhs.c0r2) + (c3r2 * rhs.c0r3);
    c0r3 = (c0r3 * rhs.c0r0) + (c1r3 * rhs.c0r1) + (c2r3 * rhs.c0r2) + (c3r3 * rhs.c0r3);

    c1r0 = (c0r0 * rhs.c1r0) + (c1r0 * rhs.c1r1) + (c2r0 * rhs.c1r2) + (c3r0 * rhs.c1r3);
    c1r1 = (c0r1 * rhs.c1r0) + (c1r1 * rhs.c1r1) + (c2r1 * rhs.c1r2) + (c3r1 * rhs.c1r3);
    c1r2 = (c0r2 * rhs.c1r0) + (c1r2 * rhs.c1r1) + (c2r2 * rhs.c1r2) + (c3r2 * rhs.c1r3);
    c1r3 = (c0r3 * rhs.c1r0) + (c1r3 * rhs.c1r1) + (c2r3 * rhs.c1r2) + (c3r3 * rhs.c1r3);

    c2r0 = (c0r0 * rhs.c2r0) + (c1r0 * rhs.c2r1) + (c2r0 * rhs.c2r2) + (c3r0 * rhs.c2r3);
    c2r1 = (c0r1 * rhs.c2r0) + (c1r1 * rhs.c2r1) + (c2r1 * rhs.c2r2) + (c3r1 * rhs.c2r3);
    c2r2 = (c0r2 * rhs.c2r0) + (c1r2 * rhs.c2r1) + (c2r2 * rhs.c2r2) + (c3r2 * rhs.c2r3);
    c2r3 = (c0r3 * rhs.c2r0) + (c1r3 * rhs.c2r1) + (c2r3 * rhs.c2r2) + (c3r3 * rhs.c2r3);

    c3r0 = (c0r0 * rhs.c3r0) + (c1r0 * rhs.c3r1) + (c2r0 * rhs.c3r2) + (c3r0 * rhs.c3r3);
    c3r1 = (c0r1 * rhs.c3r0) + (c1r1 * rhs.c3r1) + (c2r1 * rhs.c3r2) + (c3r1 * rhs.c3r3);
    c3r2 = (c0r2 * rhs.c3r0) + (c1r2 * rhs.c3r1) + (c2r2 * rhs.c3r2) + (c3r2 * rhs.c3r3);
    c3r3 = (c0r3 * rhs.c3r0) + (c1r3 * rhs.c3r1) + (c2r3 * rhs.c3r2) + (c3r3 * rhs.c3r3);

    return *this;
}

Matrix4x4 Matrix4x4::perspective(
    float aspect_ratio, float near, float far, float fov
) {
    Matrix4x4 mat;
    float scale = tan(fov * 0.5f) * near;
    float right = aspect_ratio * scale;
    float left = -right;
    float top = scale;
    float bottom = -top;

    mat.c0r0 = 2.0f * near / (right - 1.0f);
    mat.c0r1 = mat.c0r2 = mat.c0r3 = 0;

    mat.c1r1 = 2.0f * near / (top - bottom);
    mat.c1r0 = mat.c1r2 = mat.c1r3 = 0;

    mat.c2r0 = (right + left) / (right - left);
    mat.c2r1 = (top + bottom) / (top - bottom);
    mat.c2r3 = -(far + near) / (far - near);
    mat.c2r3 = -1;

    mat.c3r2 = -2.0f * far * near / (far - near);
    mat.c3r0 = mat.c3r1 = mat.c3r3 = 0;

    return mat;
}

Matrix4x4 Matrix4x4::translate(Vector4 translation) {
    Matrix4x4 mat;
    mat.c0r0 = mat.c1r1 = mat.c2r2 = 1;
    mat.c0r1 = mat.c0r2 = mat.c0r3 = 0;
    mat.c1r0 = mat.c1r2 = mat.c1r3 = 0;
    mat.c2r0 = mat.c2r1 = mat.c2r3 = 0;
    mat.c3r0 = translation.x;
    mat.c3r1 = translation.y;
    mat.c3r2 = translation.z;
    mat.c3r3 = translation.w;
    return mat;
}

Matrix4x4 Matrix4x4::scale(Vector4 scale) {
    Matrix4x4 mat;
    mat.c0r1 = mat.c0r2 = mat.c0r3 = 0;
    mat.c1r0 = mat.c1r2 = mat.c1r3 = 0;
    mat.c2r0 = mat.c2r1 = mat.c2r3 = 0;
    mat.c3r0 = mat.c3r1 = mat.c3r2 = 0;
    mat.c0r0 = scale.x;
    mat.c1r1 = scale.y;
    mat.c2r2 = scale.z;
    mat.c3r3 = scale.w;
    return mat;
}

Matrix4x4 Matrix4x4::rotation(Quaternion q) {
    Matrix4x4 mat;
    mat.c0r0 = 1.0f - 2.0f * q.y * q.y - 2 * q.z * q.z;
    mat.c0r1 = 2.0f * q.x * q.y + 2.0f * q.z * q.w;
    mat.c0r2 = 2.0f * q.x * q.z - 2.0f * q.y * q.w;

    mat.c1r0 = 2.0f * q.x * q.y - 2.0f * q.z * q.w;
    mat.c1r1 = 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z;
    mat.c1r2 = 2.0f * q.y * q.z + 2.0f * q.x * q.w;

    mat.c2r0 = 2.0f * q.x * q.z + 2.0f * q.y * q.w;
    mat.c2r1 = 2.0f * q.y * q.z - 2.0f * q.x * q.w;
    mat.c2r2 = 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y;

    mat.c3r0 = mat.c3r1 = mat.c3r2 = 0;
    mat.c1r3 = mat.c2r3 = mat.c3r3 = 0;
    mat.c3r3 = 1;
    return mat;
}
