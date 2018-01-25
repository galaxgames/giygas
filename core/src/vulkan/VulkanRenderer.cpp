#include "VulkanRenderer.hpp"

using namespace giygas;


VulkanRenderer::VulkanRenderer(shared_ptr<Context> context) {

}

VulkanRenderer::VulkanRenderer(VulkanRenderer &&) noexcept {

}

VulkanRenderer& VulkanRenderer::operator=(VulkanRenderer &&) noexcept {

    return *this;
}

void VulkanRenderer::move_common(VulkanRenderer &&other) noexcept {

}

VulkanRenderer::~VulkanRenderer() {

}

void VulkanRenderer::initialize() {

    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "Giygas Application";
    app_info.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
    app_info.pEngineName = "Giygas";
    app_info.engineVersion = VK_MAKE_VERSION(0, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;

    // TODO: Giygas VulkanContext interface needs a 'get_required_extensions' method.
    const char **needed_extensions = nullptr;
    unsigned int needed_extensions_count = 0;
    create_info.enabledExtensionCount = needed_extensions_count;
    create_info.ppEnabledExtensionNames = needed_extensions;

    // Validation layers
    create_info.enabledLayerCount = 0;

    VkResult create_result = vkCreateInstance(&create_info, nullptr, &_vulkan_instance);
}

VertexBuffer* VulkanRenderer::make_vbo() {
    return nullptr;
}

VertexArray *VulkanRenderer::make_vao() {
    return nullptr;
}

ElementBuffer<unsigned int>* VulkanRenderer::make_int_ebo() {
    return nullptr;
}

ElementBuffer<unsigned short>* VulkanRenderer::make_short_ebo() {
    return nullptr;
}

ElementBuffer<unsigned char>* VulkanRenderer::make_char_ebo() {
    return nullptr;
}

Material* VulkanRenderer::make_material() {
    return nullptr;
}

Shader* VulkanRenderer::make_shader() {
    return nullptr;
}

Texture* VulkanRenderer::make_texture(TextureInitOptions options) {
    return nullptr;
}

FrameBufferSurface *VulkanRenderer::make_framebuffer() {
    return nullptr;
}

RenderBuffer* VulkanRenderer::make_renderbuffer() {
    return nullptr;
}

Surface* VulkanRenderer::main_surface() {
    return nullptr;
}

void VulkanRenderer::present() {

}
