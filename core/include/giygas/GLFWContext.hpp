#pragma once
#include <giygas/config.hpp>

#ifdef GIYGAS_WITH_VULKAN
// Explicitly include vulkan before glfw3 to enable glfw vulkan features.
#include <vulkan/vulkan.h>
#endif

#ifdef GIYGAS_WITH_OPENGL
// Glad too, glad required it is included before any other GL headers.
#include <glad/glad.h>
#endif

#include <GLFW/glfw3.h>

#include <memory>
#include <unordered_set>
#include "GLFWWindowInitOptions.hpp"
#include "GLContext.hpp"
#include "VulkanContext.hpp"
#include <giygas/EventHandler.hpp>

namespace giygas {

    class GIYGAS_EXPORT GLFWContext final
        : public Context

#ifdef GIYGAS_WITH_OPENGL
        , public GLContext
#endif
#ifdef GIYGAS_WITH_VULKAN
        , public VulkanContext
#endif

    {
        GLFWWindowInitOptions _init_options;
        int _init_error;
        GLFWwindow *_window;
        GLVersion _version;

        Event<uint32_t, float> _input_changed;

#ifdef GIYGAS_WITH_OPENGL
        //
        // GLContext implementation
        //

        void initialize_for_opengl(GLVersion min, GLVersion max) override;
        void make_current_on_calling_thread() override;
        void present() override;
#endif //GIYGAS_WITH_OPENGL

#ifdef GIYGAS_WITH_VULKAN
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

#endif // GIYGAS_WITH_VULKAN

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
        uint32_t translate_key(InputKey key) const override;
        InputKey get_universal_key(uint32_t key) const override;
        float get_input(uint32_t input) const override;
        EventHandler<uint32_t, float> input_changed() override;


        //
        // GLFWContext implementation
        //
        void set_init_options(GLFWWindowInitOptions options);
        void show();
        void set_size(uint32_t width, uint32_t height);
    };

}
