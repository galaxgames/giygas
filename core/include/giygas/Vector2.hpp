#pragma once
#include <giygas/export.h>

namespace giygas  {
    class GIYGAS_EXPORT Vector2 {
    public:
        float x, y;

        Vector2() = default;  // Default in header to allow default value initialization
        Vector2(float x, float y);
        Vector2 operator+(const Vector2 &other) const;
        Vector2 &operator+=(const Vector2 &other);
        Vector2 operator-(const Vector2 &other) const;
        Vector2 &operator-=(const Vector2 &other);
        Vector2 operator-() const;
        Vector2 operator*(float scalar) const;
        Vector2 &operator*=(float scalar);

        Vector2 scale(const Vector2 &other) const;
    };
}

