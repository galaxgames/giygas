#include <glad/glad.h>
#include <giygas/GLFWWindow.hpp>
#include <cassert>

using namespace giygas;
using namespace std;

GLFWWindow::GLFWWindow() {
    _window = nullptr;
    _major = 1;
    _minor = 0;
    glfwInit();
}

GLFWWindow::GLFWWindow(GLFWWindow &&other) noexcept
    : _surface_size_listeners(move(other._surface_size_listeners))
{
    move_common(move(other));
}

GLFWWindow& GLFWWindow::operator=(GLFWWindow &&other) noexcept {
    _surface_size_listeners = move(other._surface_size_listeners);
    move_common(move(other));
    return *this;
}

void GLFWWindow::move_common(GLFWWindow &&other) noexcept {
    _window = other._window;
    _major = other._major;
    _minor = other._minor;
    _framebuffer_width = other._framebuffer_width;
    _framebuffer_height = other._framebuffer_height;
    other._window = nullptr;
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

    int framebuffer_width, framebuffer_height;
    glfwGetFramebufferSize(_window, &framebuffer_width, &framebuffer_height);
    _framebuffer_width = static_cast<unsigned int>(framebuffer_width);
    _framebuffer_height = static_cast<unsigned int>(framebuffer_height);

    glfwSetWindowUserPointer(_window, this);
    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

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

unsigned int GLFWWindow::framebuffer_width() const {
    return _framebuffer_width;
}

unsigned int GLFWWindow::framebuffer_height() const {
    return _framebuffer_height;
}

void GLFWWindow::add_surface_size_changed_listener(
    SurfaceSizeChangedListener *listener
) {
    assert(listener != nullptr);
    _surface_size_listeners.insert(listener);
}

void GLFWWindow::remove_surface_size_changed_listener(
        SurfaceSizeChangedListener *listener
) {
    auto it = _surface_size_listeners.find(listener);
    if (it == _surface_size_listeners.end()) {
        return;
    }
    _surface_size_listeners.erase(it);
}

void GLFWWindow::set_show_hints(int major, int minor) {
    _major = major;
    _minor = minor;
}

void GLFWWindow::framebuffer_size_callback(
    GLFWwindow *window,
    int width,
    int height
) {
    void *userdata = glfwGetWindowUserPointer(window);
    auto *instance = reinterpret_cast<GLFWWindow *>(userdata);
    instance->_framebuffer_width = static_cast<unsigned int>(width);
    instance->_framebuffer_height = static_cast<unsigned int>(height);
    for (SurfaceSizeChangedListener *listener : instance->_surface_size_listeners) {
        listener->handle_surface_size_changed(
            instance->_framebuffer_width,
            instance->_framebuffer_height
        );
    }
}
