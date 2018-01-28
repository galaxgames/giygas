#pragma once

// Explicitly include vulkan before glfw3 to enable glfw vulkan features.
#include <vulkan/vulkan.h>
// Glad too, glad required it is included before any other GL headers.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <unordered_set>
#include "EventLoopContext.hpp"
#include "GLFWWindowInitOptions.hpp"
#include "GLContext.hpp"
#include "VulkanContext.hpp"
#include <giygas/EventHandler.hpp>

namespace giygas {
    using namespace std;

    class GIYGAS_EXPORT GLFWContext
        : public EventLoopContext
        , public GLContext
        , public VulkanContext
    {
        GLFWWindowInitOptions _init_options;
        int _init_error;
        GLFWwindow *_window;
        GLVersion _version;
        unsigned int _framebuffer_width;
        unsigned int _framebuffer_height;
        Event<unsigned int, unsigned int> _surface_size_changed_event;

        void move_common(GLFWContext &&other) noexcept;
        void get_major_minor(GLVersion version, int &major, int &minor);

        GLFWwindow *try_create_opengl_window(
            const char *title,
            int width,
            int height,
            GLVersion version
        );

        static void framebuffer_size_callback(
            GLFWwindow* window,
            int width,
            int height
        );

    public:
        GLFWContext(GLFWWindowInitOptions options);
        GLFWContext(const GLFWContext &) = delete;
        GLFWContext(GLFWContext &&) noexcept;
        GLFWContext &operator=(const GLFWContext &) = delete;
        GLFWContext &operator=(GLFWContext &&) noexcept;
        virtual ~GLFWContext();

        bool is_valid() const override;
        void *cast_to_specific(RendererType type) override;
        bool supports_renderer(RendererType type) const;
        unsigned int framebuffer_width() const override;
        unsigned int framebuffer_height() const override;


        EventHandler<unsigned int, unsigned int> surface_size_changed() override;

        void update() override;
        void present() override;
        bool should_close() const override;

        // GLContext implementations
        void initialize_for_opengl(GLVersion min, GLVersion max) override;
        void make_current_on_calling_thread() override;

        // VulkanContext implementations
        bool initialize_for_vulkan() override;

        const char **get_required_instance_extensions(
            unsigned int *count
        ) const override;

        VkResult create_surface(
            VkInstance instance,
            VkSurfaceKHR *surface
        ) override;


        void show();
        void set_size(unsigned int width, unsigned int height);
    };

}
