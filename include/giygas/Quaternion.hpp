#pragma once

#include "Vector3.hpp"

namespace giygas {
    class Quaternion {
    public:
        float x, y, z, w;

        Quaternion() = default;
        Quaternion(float x, float y, float z, float w);

        Quaternion operator*(const Quaternion &rhs) const;
        Quaternion &operator*=(const Quaternion &rhs);

        /**
         * @brief angle_axis Make a quaternion from an angle and a normalized
         *        axis.
         * @param angle The angle in radians.
         * @param axis The normalized axis.
         */
        static Quaternion angle_axis(float angle, Vector3 axis);
    };
}
