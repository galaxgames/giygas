#include <giygas/Vector3.hpp>

using namespace giygas;

Vector3::Vector3() = default;
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

Vector3 Vector3::operator+(const Vector3 &rhs) const {
    Vector3 out = *this;
    return out += rhs;
}

Vector3 &Vector3::operator+=(const Vector3 &rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

Vector3 Vector3::operator*(const Vector3 &rhs) const {
    Vector3 out = *this;
    return out *= rhs;
}

Vector3 &Vector3::operator*=(const Vector3 &rhs) {
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
}

Vector3 Vector3::operator*(float rhs) const {
    Vector3 out = *this;
    return out *= rhs;
}

Vector3 &Vector3::operator*=(float rhs) {
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
}

float Vector3::dot(const Vector3 &rhs) const {
    return x * rhs.x + y * rhs.y + z * rhs.z;
}

Vector3 Vector3::cross(const Vector3 &rhs) const {
    return Vector3(
        y * rhs.z - rhs.y * z,
        z * rhs.x - rhs.z * x,
        x * rhs.y - rhs.x * y
    );
}


