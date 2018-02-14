#include "QueueFamilyIndices.hpp"

using namespace giygas;

QueueFamilyIndices::QueueFamilyIndices() {
    graphics_family = static_cast<unsigned int>(-1);
    present_family = static_cast<unsigned int>(-1);
}

bool QueueFamilyIndices::is_complete() const {
    return (
        graphics_family != static_cast<unsigned int>(-1) &&
        present_family != static_cast<unsigned int>(-1)
    );
}
