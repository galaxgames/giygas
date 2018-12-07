#include "VulkanVertexBuffer.hpp"

using namespace giygas;

void *VulkanVertexBuffer::cast_to_renderer_specific() {
    return this;
}
