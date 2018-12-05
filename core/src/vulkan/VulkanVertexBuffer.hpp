#pragma once
#include <giygas/VertexBuffer.hpp>
#include <vulkan/vulkan.h>
#include <memory>

namespace giygas {
    using namespace std;

    class VulkanRenderer;

    class VulkanVertexBuffer : public VertexBuffer {
    public:
        virtual ~VulkanVertexBuffer() = default;
        void *cast_to_renderer_specific() override;


        virtual VkBuffer handle() const = 0;

    };

}
