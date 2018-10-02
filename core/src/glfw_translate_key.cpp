#include "glfw_translate_key.hpp"
#include <GLFW/glfw3.h>

using namespace giygas;

uint32_t giygas::translate_to_glfw3_key(InputKey key) {
    #define KEY_MAPPING(k, v) case k: return v;
    switch (key) {
        #include "glfw_key_mappings.hpp"
        default:
            return static_cast<uint32_t>(GLFW_KEY_UNKNOWN);
    }
    #undef KEY_MAPPING
}

InputKey giygas::translate_to_giygas_key(uint32_t key) {
    #define KEY_MAPPING(v, k) case k: return v;
    switch (key) {
    #include "glfw_key_mappings.hpp"
        default:
            return InputKey::UNKNOWN;
    }
    #undef KEY_MAPPING
}