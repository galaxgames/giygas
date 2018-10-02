#pragma once
#include <giygas/InputKey.hpp>
#include <cstdint>

namespace giygas {
    uint32_t translate_to_glfw3_key(InputKey key);
    InputKey translate_to_giygas_key(uint32_t key);
}