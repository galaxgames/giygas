#pragma once
#include "Vector4.hpp"
#include "Quaternion.hpp"

namespace giygas {
    class GIYGAS_EXPORT Matrix4x4 {
    public:
        float c0r0, c0r1, c0r2, c0r3;
        float c1r0, c1r1, c1r2, c1r3;
        float c2r0, c2r1, c2r2, c2r3;
        float c3r0, c3r1, c3r2, c3r3;

        Matrix4x4() = default;

        Matrix4x4 operator*(const Matrix4x4 &other) const;
        Matrix4x4 &operator*=(const Matrix4x4 &other);

        static Matrix4x4 translate(Vector4 translation);
        static Matrix4x4 scale(Vector4 scale);
        static Matrix4x4 rotation(Quaternion q);
    };
}
