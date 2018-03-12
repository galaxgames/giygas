#include <giygas/Vector2.hpp>

using namespace giygas;

Vector2::Vector2(float x, float y) : x(x), y(y) {}

Vector2 Vector2::operator+(const Vector2 &other) const {
    Vector2 result = *this;
    return result += other;
}

Vector2& Vector2::operator+=(const Vector2 &other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vector2 Vector2::operator*(float scalar) const {
    Vector2 result = *this;
    return result *= scalar;
}

Vector2& Vector2::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}