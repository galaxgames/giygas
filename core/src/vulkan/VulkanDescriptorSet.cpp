#include "VulkanDescriptorSet.hpp"
#include "VulkanRenderer.hpp"
#include "VulkanSampler.hpp"
#include "VulkanUniformBuffer.hpp"
#include "VulkanTexture.hpp"

using namespace giygas;


class DescriptorSetSafeDeletable final : public SwapchainSafeDeleteable {

    VkDescriptorSetLayout _layout;

public:

    DescriptorSetSafeDeletable(VkDescriptorSetLayout layout) {
        _layout = layout;
    }

    void delete_resources(VulkanRenderer &renderer) override {
        // Don't need to and shouldn't free descriptor set since we haven't told the pool we want to
        // be able to do that. Instead the pool will take care of freeing the descriptor sets.
        //vkFreeDescriptorSets(device, _pool, 1, &_handle);

        vkDestroyDescriptorSetLayout(renderer.device(), _layout, nullptr);
    }

};


VulkanDescriptorSet::VulkanDescriptorSet(VulkanRenderer *renderer) {
    _renderer = renderer;
    _layout = VK_NULL_HANDLE;
    _handle = VK_NULL_HANDLE;
    _pool = VK_NULL_HANDLE;
    _uniform_buffer_count = 0;
    _sampler_count = 0;
    _has_descriptors = false;
}

VulkanDescriptorSet::~VulkanDescriptorSet() {
    _renderer->delete_when_safe(unique_ptr<SwapchainSafeDeleteable>(new DescriptorSetSafeDeletable(_layout)));
    _layout = VK_NULL_HANDLE;
}

RendererType VulkanDescriptorSet::renderer_type() const {
    return RendererType::Vulkan;
}

bool VulkanDescriptorSet::is_created() const {
    return _handle != VK_NULL_HANDLE;
}

bool VulkanDescriptorSet::has_descriptors() const {
    return _has_descriptors;
}

void VulkanDescriptorSet::create(const DescriptorSetCreateParameters &params) {
    // TODO: Validation!!
    assert(params.pool != nullptr);
    assert(params.pool->renderer_type() == RendererType::Vulkan);
    const auto *pool = reinterpret_cast<const VulkanDescriptorPool *>(params.pool);
    create(pool, params);
}

void VulkanDescriptorSet::create(
    const VulkanDescriptorPool *pool,
    const DescriptorSetCreateParameters &params
) {
    _pool = pool->handle();
    assert(_pool != VK_NULL_HANDLE);

    VkDevice device = _renderer->device();

//    assert(params.immutable_sampler_count == 0 || params.immutable_sampler_count == params.sampler_count);
//    unique_ptr<VkSampler[]> immutable_sampler_handles(new VkSampler[params.immutable_sampler_count]);
//    for (size_t i = 0; i < params.immutable_sampler_count; ++i) {
//        const Sampler *sampler = params.immutable_samplers[i];
//        assert(sampler->renderer_type() == RendererType::Vulkan);
//        const auto *sampler_impl = reinterpret_cast<const VulkanSampler *>(sampler);
//        immutable_sampler_handles[i] = sampler_impl->handle();
//    }

    uint32_t binding_count = params.uniform_buffer_count + params.sampler_count;
    uint32_t binding_index = 0;
    unique_ptr<VkDescriptorSetLayoutBinding[]> bindings(
        new VkDescriptorSetLayoutBinding[binding_count] {}
    );
    unique_ptr<VkSampler[]> sampler_handles(new VkSampler[params.sampler_count] {});

    for (uint32_t i = 0; i < params.uniform_buffer_count; ++i) {
        VkDescriptorSetLayoutBinding &binding = bindings[binding_index++];
        const UniformBufferDescriptorSlot &slot = params.uniform_buffer_slots[i];
        binding.binding = slot.binding_index;
        binding.descriptorCount = 1;
        binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        binding.stageFlags = translate_shader_stages(slot.stages);
    }
    for (uint32_t i = 0; i < params.sampler_count; ++i) {
        VkDescriptorSetLayoutBinding &binding = bindings[binding_index++];
        const SamplerDescriptorSlot &slot = params.sampler_slots[i];
        binding.binding = slot.binding_index;
        binding.descriptorCount = 1;
        binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        if (slot.immutable_sampler != nullptr) {
            assert(slot.immutable_sampler->renderer_type() == RendererType::Vulkan);
            const auto *sampler_impl = reinterpret_cast<const VulkanSampler *>(slot.immutable_sampler);
            sampler_handles[i] = sampler_impl->handle();
            binding.pImmutableSamplers = &sampler_handles[i];
        }
        binding.stageFlags = translate_shader_stages(slot.stages);
    }

    VkDescriptorSetLayoutCreateInfo layout_info = {};
    layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_info.bindingCount = binding_count;
    layout_info.pBindings = bindings.get();

    vkCreateDescriptorSetLayout(device, &layout_info, nullptr, &_layout);

    VkDescriptorSetAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool = _pool;
    alloc_info.descriptorSetCount = 1;
    alloc_info.pSetLayouts = &_layout;

    vkAllocateDescriptorSets(device, &alloc_info, &_handle);

    _uniform_buffer_count = params.uniform_buffer_count;
    _sampler_count = params.sampler_count;
}

void VulkanDescriptorSet::update(const DescriptorSetUpdateParameters &params) {
    // TODO: Validation layer!
        assert(is_created());
    // TODO: Need beter validation than this.
    assert(params.uniform_buffer_count == _uniform_buffer_count);
    assert(params.sampler_count == _sampler_count);
    
    VkDevice device = _renderer->device();

    unique_ptr<VkDescriptorBufferInfo[]> buffer_infos(
        new VkDescriptorBufferInfo[params.uniform_buffer_count] {}
    );
    unique_ptr<VkDescriptorImageInfo[]> image_infos(
        new VkDescriptorImageInfo[params.sampler_count] {}
    );

//    for (size_t i = 0; i < params.uniform_buffer_count; ++i) {
//        VkDescriptorBufferInfo &buffer_info = buffer_infos[i];
//        const UniformBufferDescriptorBinding &binding = params.uniform_buffer_bindings[i];
//        assert(binding.buffer != nullptr);
//        assert(binding.buffer->renderer_type() == RendererType::Vulkan);
//        const auto *buffer_impl = reinterpret_cast<const VulkanUniformBuffer *>(binding.buffer);
//        buffer_info.buffer = buffer_impl->handle();
//        buffer_info.offset = 0;
//        buffer_info.range = buffer_impl->size();
//    }

//    for (size_t i = 0; i < params.sampler_count; ++i) {
//        VkDescriptorImageInfo &image_info = image_infos[i];
//        const SamplerDescriptorBinding &binding = params.sampler_bindings[i];
//        const Texture *texture = binding.texture;
//        assert(texture != nullptr);
//        assert(texture->renderer_type() == RendererType::Vulkan);
//        const auto *texture_impl = static_cast<const VulkanTexture *>(texture->texture_impl());
//        assert(texture_impl->image_view() != VK_NULL_HANDLE);
//        const Sampler *sampler = binding.sampler;
//        assert(sampler->renderer_type() == RendererType::Vulkan);
//        const auto *sampler_impl = reinterpret_cast<const VulkanSampler *>(sampler);
//        image_info.imageLayout = texture_impl->layout();
//        image_info.sampler = sampler_impl->handle();
//        image_info.imageView = texture_impl->image_view();
//    }

    uint32_t write_count = params.uniform_buffer_count + params.sampler_count;
    unique_ptr<VkWriteDescriptorSet[]> writes (new VkWriteDescriptorSet[write_count] {});
    uint32_t write_index = 0;

    for (uint32_t i = 0; i < params.uniform_buffer_count; ++i) {
        const UniformBufferDescriptorBinding &binding = params.uniform_buffer_bindings[i];

        VkDescriptorBufferInfo &buffer_info = buffer_infos[i];
        assert(binding.buffer != nullptr);
        assert(binding.buffer->renderer_type() == RendererType::Vulkan);
        const auto *buffer_impl = reinterpret_cast<const VulkanUniformBuffer *>(binding.buffer);
        buffer_info.buffer = buffer_impl->handle();
        buffer_info.offset = 0;
        buffer_info.range = buffer_impl->size();

        VkWriteDescriptorSet &write = writes[write_index++];
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = _handle;
        write.dstBinding = binding.binding_index;
        write.dstArrayElement = 0;
        write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        write.descriptorCount = 1;
        write.pBufferInfo = &buffer_info;
    }
    for (size_t i = 0; i < params.sampler_count; ++i) {
        const SamplerDescriptorBinding &binding = params.sampler_bindings[i];

        VkDescriptorImageInfo &image_info = image_infos[i];
        const Texture *texture = binding.texture;
        assert(texture != nullptr);
        assert(texture->renderer_type() == RendererType::Vulkan);
        const auto *texture_impl = static_cast<const VulkanTexture *>(texture->texture_impl());
        assert(texture_impl->image_view(0) != VK_NULL_HANDLE);
        const Sampler *sampler = binding.sampler;
        assert(sampler->renderer_type() == RendererType::Vulkan);
        const auto *sampler_impl = reinterpret_cast<const VulkanSampler *>(sampler);
        image_info.imageLayout = texture_impl->layout();
        image_info.sampler = sampler_impl->handle();
        image_info.imageView = texture_impl->image_view(0);

        VkWriteDescriptorSet &write = writes[write_index++];
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = _handle;
        write.dstBinding = binding.binding_index;
        write.dstArrayElement = 0;
        write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        write.descriptorCount = 1;
        write.pImageInfo = &image_info;
    }

    vkUpdateDescriptorSets(
        device,
        write_count,
        writes.get(),
        0,  //descriptor copy count
        nullptr  // pdescriptorcopies
    );

    _has_descriptors = true;
}

VkDescriptorSetLayout VulkanDescriptorSet::layout() const {
    return _layout;
}

VkDescriptorSet VulkanDescriptorSet::handle() const {
    return _handle;
}

VkShaderStageFlags VulkanDescriptorSet::translate_shader_stages(ShaderStage stages) const {
    VkShaderStageFlags flags = 0;
    if (stages & GIYGAS_SHADER_STAGE_VERTEX) {
        flags |= VK_SHADER_STAGE_VERTEX_BIT;
    }
    if (stages & GIYGAS_SHADER_STAGE_FRAGMENT) {
        flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
    }
    return flags;
}
