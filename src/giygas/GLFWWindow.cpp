#include <glad/glad.h>
#include <giygas/GLFWWindow.hpp>
#include <cassert>

using namespace giygas;
using namespace std;

GLFWWindow::GLFWWindow() {
    _window = nullptr;
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
    _version = other._version;
    _framebuffer_width = other._framebuffer_width;
    _framebuffer_height = other._framebuffer_height;
    other._window = nullptr;
}

GLFWWindow::~GLFWWindow() {
    glfwTerminate();
}

void GLFWWindow::initialize(GLFWWindowInitOptions options) {
    glfwWindowHint(GLFW_RESIZABLE, options.is_resizable ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

    _version = options.gl_max_version;
    while (_version >= options.gl_min_version) {
        _window = try_create_window(
            options.title,
            options.width,
            options.height,
            _version
        );
        if (_window != nullptr) {
            break;
        }
        auto discriminant = static_cast<underlying_type<GLVersion>::type>(_version);
        --discriminant;
        _version = static_cast<GLVersion>(discriminant);
    }

    if (_window == nullptr) {
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

GLFWwindow *GLFWWindow::try_create_window(
    const char *title,
    int width,
    int height,
    GLVersion version
) {
    int major, minor;
    get_major_minor(version, major, minor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    return glfwCreateWindow(width, height, title, nullptr, nullptr);
}

void GLFWWindow::get_major_minor(GLVersion version, int &major, int &minor) {
    GLVersion base;
    int offset;
    switch(version) {
        case GLVersion::GL_3_2:
        case GLVersion::GL_3_3:
            major = 3;
            base = GLVersion::GL_3_2;
            offset = 2;
            break;
        case GLVersion::GL_4_0:
        case GLVersion::GL_4_1:
        case GLVersion::GL_4_2:
        case GLVersion::GL_4_3:
        case GLVersion::GL_4_4:
        case GLVersion::GL_4_5:
        case GLVersion::GL_4_6:
            major = 4;
            base = GLVersion::GL_4_0;
            offset = 0;
            break;
    }
    underlying_type<GLVersion>::type discriminant, base_discriminant;
    discriminant = static_cast<underlying_type<GLVersion>::type>(version);
    base_discriminant = static_cast<underlying_type<GLVersion>::type>(base);
    minor = discriminant - base_discriminant + offset;
}

void GLFWWindow::show() {
    glfwShowWindow(_window);
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
