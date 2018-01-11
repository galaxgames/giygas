#pragma once

namespace giygasfile {
    enum class AttributeUsageHint : unsigned char {
        NO_HINT = 0,
        POSITION = 1,
        COLOR = 2,
        NORMALS = 3,
        TEXTURE_COORDINATES_0 = 4,
        TEXTURE_COORDINATES_1 = 5,
        TEXTURE_COORDINATES_2 = 6,
        TEXTURE_COORDINATES_3 = 7
    };
}
