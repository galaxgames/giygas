#include "VulkanDescriptorPool.hpp"
#include "VulkanRenderer.hpp"

using namespace giygas;

VulkanDescriptorPool::VulkanDescriptorPool(VulkanRenderer *renderer) {
    _renderer = renderer;
    _handle = VK_NULL_HANDLE;
}

VulkanDescriptorPool::~VulkanDescriptorPool() {
    vkDestroyDescriptorPool(_renderer->device(), _handle, nullptr);
}

RendererType VulkanDescriptorPool::renderer_type() const {
    return RendererType::Vulkan;
}

void VulkanDescriptorPool::create(const DescriptorPoolParameters &params) {
    array<VkDescriptorPoolSize, 2> sizes = {};
    sizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    sizes[0].descriptorCount = max<uint32_t>(params.uniform_buffer_descriptors, 1);
    sizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    sizes[1].descriptorCount = max<uint32_t>(params.sampler_descriptors, 1);

    VkDescriptorPoolCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    create_info.poolSizeCount = sizes.size();
    create_info.pPoolSizes = sizes.data();
    create_info.maxSets = params.max_sets;

    vkCreateDescriptorPool(_renderer->device(), &create_info, nullptr, &_handle);
}

VkDescriptorPool VulkanDescriptorPool::handle() const {
    return _handle;
}