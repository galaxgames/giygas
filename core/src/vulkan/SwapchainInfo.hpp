#pragma once
#include <memory>

namespace giygas {
    using namespace std;

    class SwapchainInfo {
    public:
        VkSurfaceCapabilitiesKHR capabilities;
        uint32_t format_count;
        uint32_t present_mode_count;
        unique_ptr<VkSurfaceFormatKHR[]> formats;
        unique_ptr<VkPresentModeKHR[]> present_modes;
    };
}
