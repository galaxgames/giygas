#pragma once
#include "VulkanRenderer.hpp"

namespace giygas {

    class SwapchainSafeDeleteable
    {
    public:
        virtual ~SwapchainSafeDeleteable() = default;
        virtual void delete_resources(VulkanRenderer &renderer) = 0;
    };

}

