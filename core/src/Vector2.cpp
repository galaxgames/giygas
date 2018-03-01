#include <giygas/Vector2.hpp>

using namespace giygas;

Vector2::Vector2(float x, float y) : x(x), y(y) {}

Vector2 Vector2::operator+(const Vector2 &other) {
    Vector2 result = other;
    return result += *this;
}

Vector2& Vector2::operator+=(const Vector2 &other) {
    x += other.x;
    y += other.y;
    return *this;
}