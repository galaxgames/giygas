#pragma once
#include <giygas/Renderer.hpp>
#include <giygas/Context.hpp>
#include <vulkan/vulkan.h>


namespace giygas {

    class VulkanRenderer : public Renderer {
        VkInstance _vulkan_instance;

        void move_common(VulkanRenderer &&other) noexcept;

    public:
        VulkanRenderer(shared_ptr<Context> context);
        VulkanRenderer(const VulkanRenderer &) = delete;
        VulkanRenderer &operator=(const VulkanRenderer &) = delete;
        VulkanRenderer(VulkanRenderer &&) noexcept;
        VulkanRenderer &operator=(VulkanRenderer &&) noexcept;
        ~VulkanRenderer();

        void initialize() override;

        VertexBuffer *make_vbo() override;
        VertexArray *make_vao() override;
        ElementBuffer<unsigned int> *make_int_ebo() override;
        ElementBuffer<unsigned short> *make_short_ebo() override;
        ElementBuffer<unsigned char> *make_char_ebo() override;
        Material *make_material() override;
        Shader *make_shader() override;
        Texture *make_texture(TextureInitOptions options) override;
        FrameBufferSurface *make_framebuffer() override;
        RenderBuffer *make_renderbuffer() override;
        Surface *main_surface() override;

        void present() override;
    };

}