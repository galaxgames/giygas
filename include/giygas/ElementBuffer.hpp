#pragma once
#include <giygas/export.h>

namespace giygas {
    class GIYGAS_EXPORT ElementBuffer {
    public:
        virtual ~ElementBuffer() = default;

        virtual void set(int index, const unsigned int *elements, int count) = 0;
    };
}
