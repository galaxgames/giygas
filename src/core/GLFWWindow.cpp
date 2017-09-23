#include <glad/glad.h>
#include "giygas/GLFWWindow.hpp"

using namespace giygas;

GLFWWindow::GLFWWindow() {
    _window = nullptr;
    _major = 1;
    _minor = 0;
    glfwInit();
}

GLFWWindow::GLFWWindow(GLFWWindow &&other) noexcept{
    *this = std::move(other);
}

GLFWWindow& GLFWWindow::operator=(GLFWWindow &&other) noexcept {
    _window = other._window;
    _major = other._major;
    _minor = other._minor;
    return *this;
}

GLFWWindow::~GLFWWindow() {
    glfwTerminate();
}

void GLFWWindow::show() {
    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _minor);
    _window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!_window) {
        return;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(_window);
}

void GLFWWindow::update() {
    glfwSwapBuffers(_window);
    glfwPollEvents();
}

bool GLFWWindow::should_close() const {
    return static_cast<bool>(glfwWindowShouldClose(_window));
}

bool GLFWWindow::is_valid() const {
    return _window != nullptr;
}

void GLFWWindow::set_show_hints(int major, int minor) {
    _major = major;
    _minor = minor;
}