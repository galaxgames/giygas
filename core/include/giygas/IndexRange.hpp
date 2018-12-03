#pragma once
#include <giygas/export.h>
#include <cstdint>

namespace giygas {

    class GIYGAS_EXPORT IndexRange {
    public:
        uint32_t offset;
        uint32_t count;
    };

}
