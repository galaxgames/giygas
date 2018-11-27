#pragma once
#include <cstdint>

namespace giygas {
    class QueueFamilyIndices {
    public:
        uint32_t graphics_family = static_cast<unsigned int>(-1);
        uint32_t present_family = static_cast<unsigned int>(-1);

        bool is_complete() const;
    };
}
