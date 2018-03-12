#pragma once

// Explicitly include vulkan before glfw3 to enable glfw vulkan features.
#include <vulkan/vulkan.h>
// Glad too, glad required it is included before any other GL headers.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <unordered_set>
#include "GLFWWindowInitOptions.hpp"
#include "GLContext.hpp"
#include "VulkanContext.hpp"
#include <giygas/EventHandler.hpp>

namespace giygas {
    using namespace std;

    class GIYGAS_EXPORT GLFWContext final
        : public Context
        , public GLContext
        , public VulkanContext
    {
        GLFWWindowInitOptions _init_options;
        int _init_error;
        GLFWwindow *_window;
        GLVersion _version;

        Event<unsigned int, float> _input_changed;


        //
        // GLContext implementation
        //

        void initialize_for_opengl(GLVersion min, GLVersion max) override;
        void make_current_on_calling_thread() override;
        void present() override;


        //
        // VulkanContext implementation
        //

        bool initialize_for_vulkan() override;

        const char **get_required_instance_extensions(
            unsigned int *count
        ) const override;

        VkResult create_surface(
            VkInstance instance,
            VkSurfaceKHR *surface
        ) override;


        //
        // GLFWContext implementation
        //

        void move_common(GLFWContext &&other) noexcept;
        void get_major_minor(GLVersion version, int &major, int &minor);

        GLFWwindow *try_create_opengl_window(
            const char *title,
            int width,
            int height,
            GLVersion version
        );

        void setup_callbacks();

        static void framebuffer_size_callback(
            GLFWwindow* window,
            int width,
            int height
        );

        static void key_callback(
            GLFWwindow* window,
            int key,
            int scancode,
            int action,
            int mods
        );

    public:
        GLFWContext();
        GLFWContext(const GLFWContext &) = delete;
        GLFWContext(GLFWContext &&) noexcept;
        GLFWContext &operator=(const GLFWContext &) = delete;
        GLFWContext &operator=(GLFWContext &&) noexcept;
        ~GLFWContext() override;

        //
        // Context implementation
        //

        bool is_valid() const override;
        void *cast_to_specific(RendererType type) override;
        void update() override;
        bool should_close() const override;
        unsigned int translate_key(InputKey key) const override;
        EventHandler<unsigned int, float> input_changed() override;


        //
        // GLFWContext implementation
        //
        void set_init_options(GLFWWindowInitOptions options);
        void show();
        void set_size(unsigned int width, unsigned int height);
    };

}
