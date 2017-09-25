#include <giygas/Quaternion.hpp>
#include <cmath>

using namespace giygas;
using namespace std;

Quaternion::Quaternion(float x, float y, float z, float w)
    : x(x), y(y), z(z), w(w)
{
}

Quaternion Quaternion::operator*(const Quaternion &rhs) const {
    Quaternion out = *this;
    return out *= rhs;
}

Quaternion &Quaternion::operator*=(const Quaternion &rhs) {
    // TODO: This could be optimized
    float scalar_l = w;
    float scalar_r = rhs.w;
    Vector3 vector_l(x, y, z);
    Vector3 vector_r(rhs.x, rhs.y, rhs.z);
    w = scalar_l * scalar_r - vector_l.dot(vector_r);
    Vector3 vector_o =
        vector_l.cross(vector_r)
        + vector_r * scalar_l
        + vector_l * scalar_r;
    x = vector_o.x;
    y = vector_o.y;
    z = vector_o.z;
    return *this;
}

Quaternion Quaternion::angle_axis(float angle, Vector3 axis) {
    float half_angle = angle * 0.5f;
    float half_angle_sin = sin(half_angle);
    return Quaternion(
        axis.x * half_angle_sin,
        axis.y * half_angle_sin,
        axis.z * half_angle_sin,
        cos(half_angle)
    );
}
