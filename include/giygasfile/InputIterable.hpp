#pragma once
#include <cstddef>
#include <giygas/export.h>

namespace giygasfile {
    class GIYGAS_EXPORT InputIterable {
    public:
        virtual ~InputIterable() = default;
        virtual size_t read(size_t count, void *out) = 0;
    };
}
