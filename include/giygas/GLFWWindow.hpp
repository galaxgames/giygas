#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <unordered_set>
#include "Window.hpp"

namespace giygas {

    class GIYGAS_EXPORT GLFWWindow : public Window {
        GLFWwindow *_window;
        int _major;
        int _minor;
        unsigned int _framebuffer_width;
        unsigned int _framebuffer_height;
        std::unordered_set<SurfaceSizeChangedListener *> _surface_size_listeners;

        void move_common(GLFWWindow &&other) noexcept;
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    public:
        GLFWWindow();
        GLFWWindow(const GLFWWindow &) = delete;
        GLFWWindow(GLFWWindow &&) noexcept;
        GLFWWindow &operator=(const GLFWWindow &) = delete;
        GLFWWindow &operator=(GLFWWindow &&) noexcept;
        virtual ~GLFWWindow();

        void show() override;
        void update() override;
        bool should_close() const override;
        bool is_valid() const override;
        unsigned int framebuffer_width() const override;
        unsigned int framebuffer_height() const override;
        void add_surface_size_changed_listener(
            SurfaceSizeChangedListener *listener
        ) override;
        void remove_surface_size_changed_listener(
            SurfaceSizeChangedListener *listener
        ) override;

        void set_show_hints(int major, int minor);
    };

}
