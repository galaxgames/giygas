#pragma once
#include <cstddef>
#include <giygasfile/export.h>

namespace giygasfile {
    class GIYGASFILE_EXPORT InputIterable {
    public:
        virtual ~InputIterable() = default;
        virtual size_t read(size_t count, void *out) = 0;
    };
}
