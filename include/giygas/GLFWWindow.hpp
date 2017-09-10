#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "Window.hpp"

namespace giygas {

    class GIYGAS_EXPORT GLFWWindow : public Window {
        GLFWwindow *_window;
        int _major;
        int _minor;

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

        void set_show_hints(int major, int minor);
    };

}
