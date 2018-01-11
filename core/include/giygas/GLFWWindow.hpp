#pragma once
#include <memory>
#include <unordered_set>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "EventLoopContext.hpp"
#include "GLFWWindowInitOptions.hpp"
#include "GLContext.hpp"

namespace giygas {
    using namespace std;

    class GIYGAS_EXPORT GLFWWindow : public EventLoopContext, public GLContext {
        GLFWwindow *_window;
        GLVersion _version;
        unsigned int _framebuffer_width;
        unsigned int _framebuffer_height;
        unordered_set<SurfaceSizeChangedListener *> _surface_size_listeners;

        void move_common(GLFWWindow &&other) noexcept;
        void get_major_minor(GLVersion version, int &major, int &minor);

        GLFWwindow *try_create_window(
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
        GLFWWindow();
        GLFWWindow(const GLFWWindow &) = delete;
        GLFWWindow(GLFWWindow &&) noexcept;
        GLFWWindow &operator=(const GLFWWindow &) = delete;
        GLFWWindow &operator=(GLFWWindow &&) noexcept;
        virtual ~GLFWWindow();

        bool is_valid() const override;
        void *cast_to_specific(RendererType type) override;
        bool supports_renderer(RendererType type) const;
        unsigned int framebuffer_width() const override;
        unsigned int framebuffer_height() const override;

        void add_surface_size_changed_listener(
            SurfaceSizeChangedListener *listener
        ) override;

        void remove_surface_size_changed_listener(
            SurfaceSizeChangedListener *listener
        ) override;

        void update() override;
        void present() override;
        bool should_close() const override;

        void make_current_on_calling_thread() override;

        void initialize(const GLFWWindowInitOptions options);
        void show();
        void set_size(unsigned int width, unsigned int height);
    };

}
