#pragma once
#include <giygas/export.h>
//#include <giygas/Vector4.hpp>

namespace giygas {
    class GIYGAS_EXPORT Vector3 {
    public:
        float x, y, z;

        Vector3();
        Vector3(float x, float y, float z);

        Vector3 operator+(const Vector3 &rhs) const;
        Vector3 &operator+=(const Vector3 &rhs);
        Vector3 operator*(const Vector3 &rhs) const;
        Vector3 &operator*=(const Vector3 &rhs);
        Vector3 operator*(float rhs) const;
        Vector3 &operator*=(float rhs);

        float dot(const Vector3 &rhs) const;
        Vector3 cross(const Vector3 &rhs) const;
    };

}
