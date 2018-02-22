#include "VulkanDescriptorSet.hpp"
#include "VulkanRenderer.hpp"
#include "VulkanSampler.hpp"
#include "VulkanUniformBuffer.hpp"
#include "VulkanTexture.hpp"

using namespace giygas;

VulkanDescriptorSet::VulkanDescriptorSet(VulkanRenderer *renderer) {
    _renderer = renderer;
    _layout = VK_NULL_HANDLE;
    _handle = VK_NULL_HANDLE;
    _pool = VK_NULL_HANDLE;
}

VulkanDescriptorSet::~VulkanDescriptorSet() {
    VkDevice device = _renderer->device();

    // Don't need to and shouldn't free descriptor set since we haven't told the pool we want to
    // be able to do that. Instead the pool will take care of freeing the descriptor sets.
    //vkFreeDescriptorSets(device, _pool, 1, &_handle);

    vkDestroyDescriptorSetLayout(device, _layout, nullptr);
}

RendererType VulkanDescriptorSet::renderer_type() const {
    return RendererType::Vulkan;
}

void VulkanDescriptorSet::create(const DescriptorSetParameters &params) {
    assert(params.pool != nullptr);
    assert(params.pool->renderer_type() == RendererType::Vulkan);
    const auto *pool = reinterpret_cast<const VulkanDescriptorPool *>(params.pool);
    create(pool, params);
}

void VulkanDescriptorSet::create(
    const VulkanDescriptorPool *pool,
    const DescriptorSetParameters &params
) {
    _pool = pool->handle();
    assert(_pool != VK_NULL_HANDLE);

    VkDevice device = _renderer->device();

    unique_ptr<VkSampler[]> sampler_handles(new VkSampler[params.sampler_count]);
    for (size_t i = 0; i < params.sampler_count; ++i) {
        const Sampler *sampler = params.samplers[i];
        assert(sampler->renderer_type() == RendererType::Vulkan);
        const auto *sampler_impl = reinterpret_cast<const VulkanSampler *>(sampler);
        sampler_handles[i] = sampler_impl->handle();
    }

    uint32_t binding_count = 0;
    array<VkDescriptorSetLayoutBinding, 2> bindings = {};

    if (params.uniform_buffer_count > 0) {
        VkDescriptorSetLayoutBinding &binding = bindings[binding_count];
        binding.binding = static_cast<uint32_t>(binding_count);
        binding.descriptorCount = static_cast<uint32_t>(params.uniform_buffer_count);
        binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        binding.pImmutableSamplers = nullptr;
        binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;  // TODO
        ++binding_count;
    }
    if (params.sampler_count > 0) {
        VkDescriptorSetLayoutBinding &binding = bindings[binding_count];
        binding.binding = static_cast<uint32_t>(binding_count);
        binding.descriptorCount = static_cast<uint32_t>(params.sampler_count);
        binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        binding.pImmutableSamplers = sampler_handles.get();
        binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;  // TODO
        ++binding_count;
    }

    VkDescriptorSetLayoutCreateInfo layout_info = {};
    layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_info.bindingCount = binding_count;
    layout_info.pBindings = bindings.data();

    vkCreateDescriptorSetLayout(device, &layout_info, nullptr, &_layout);

    VkDescriptorSetAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool = _pool;
    alloc_info.descriptorSetCount = 1;
    alloc_info.pSetLayouts = &_layout;

    vkAllocateDescriptorSets(device, &alloc_info, &_handle);

    unique_ptr<VkDescriptorBufferInfo[]> buffer_infos(
        new VkDescriptorBufferInfo[params.uniform_buffer_count] {}
    );
    unique_ptr<VkDescriptorImageInfo[]> image_infos(
        new VkDescriptorImageInfo[params.sampler_count] {}
    );

    for (size_t i = 0; i < params.uniform_buffer_count; ++i) {
        VkDescriptorBufferInfo &buffer_info = buffer_infos[i];
        const UniformBuffer *buffer = params.uniform_buffers[i];
        assert(buffer != nullptr);
        assert(buffer->renderer_type() == RendererType::Vulkan);
        const auto *buffer_impl = reinterpret_cast<const VulkanUniformBuffer *>(buffer);
        buffer_info.buffer = buffer_impl->handle();
        buffer_info.offset = 0;
        buffer_info.range = buffer_impl->size();
    }

    for (size_t i = 0; i < params.sampler_count; ++i) {
        VkDescriptorImageInfo &image_info = image_infos[i];
        const Texture *texture = params.textures[i];
        assert(texture != nullptr);
        assert(texture->renderer_type() == RendererType::Vulkan);
        const auto *texture_impl = static_cast<const VulkanTexture *>(texture->texture_impl());
        assert(texture_impl->image_view() != VK_NULL_HANDLE);
        image_info.imageLayout = texture_impl->layout();
        image_info.sampler = sampler_handles[i];
        image_info.imageView = texture_impl->image_view();
    }


    binding_count = 0;
    array<VkWriteDescriptorSet, 2> writes = {};

    if (params.uniform_buffer_count > 0) {
        VkWriteDescriptorSet &write = writes[binding_count];
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = _handle;
        write.dstBinding = static_cast<uint32_t>(binding_count);
        write.dstArrayElement = 0;
        write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        write.descriptorCount = static_cast<uint32_t>(params.uniform_buffer_count);
        write.pBufferInfo = buffer_infos.get();
        ++binding_count;
    }
    if (params.sampler_count > 0) {
        VkWriteDescriptorSet &write = writes[binding_count];
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = _handle;
        write.dstBinding = static_cast<uint32_t>(binding_count);
        write.dstArrayElement = 0;
        write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        write.descriptorCount = static_cast<uint32_t>(params.sampler_count);
        write.pImageInfo = image_infos.get();
        ++binding_count;
    }

    vkUpdateDescriptorSets(
        device,
        binding_count,
        writes.data(),
        0,  //descriptor copy count
        nullptr  // pdescriptorcopies
    );
}

VkDescriptorSetLayout VulkanDescriptorSet::layout() const {
    return _layout;
}

VkDescriptorSet VulkanDescriptorSet::handle() const {
    return _handle;
}
