#pragma once
#include <cstddef>

namespace giygasfile {
    class InputIterable {
    public:
        virtual ~InputIterable() = default;
        virtual size_t read(size_t count, void *out) = 0;
    };
}
