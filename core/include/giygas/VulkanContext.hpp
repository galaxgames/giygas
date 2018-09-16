#pragma once
#include <giygas/config.hpp>
#ifdef GIYGAS_WITH_VULKAN
#include <giygas/export.h>
#include <vulkan/vulkan.h>
#include "Context.hpp"

namespace giygas {
    class GIYGAS_EXPORT VulkanContext {
    public:
        virtual bool initialize_for_vulkan() = 0;
        virtual const char **get_required_instance_extensions(
            unsigned int *count
        ) const = 0;
        virtual VkResult create_surface(
            VkInstance instance,
            VkSurfaceKHR *surface
        ) = 0;
    };
}

#endif