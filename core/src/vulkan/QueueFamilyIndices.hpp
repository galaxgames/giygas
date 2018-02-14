#pragma once
#include <cstdint>

namespace giygas {
    class QueueFamilyIndices {
    public:
        uint32_t graphics_family;
        uint32_t present_family;

        QueueFamilyIndices();
        bool is_complete() const;
    };
}
