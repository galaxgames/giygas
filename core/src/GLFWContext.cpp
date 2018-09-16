#ifdef GIYGAS_WITH_OPENGL
#include <glad/glad.h>
#endif
#include <giygas/GLFWContext.hpp>
#include "glfw_translate_key.hpp"

using namespace giygas;
using namespace std;

GLFWContext::GLFWContext() {
    _window = nullptr;
    _init_error = glfwInit();
}

GLFWContext::GLFWContext(GLFWContext &&other) noexcept
    : _input_changed(move(other._input_changed))
{
    move_common(move(other));
}

GLFWContext& GLFWContext::operator=(GLFWContext &&other) noexcept {
    _input_changed = move(other._input_changed);
    move_common(move(other));
    return *this;
}

void GLFWContext::move_common(GLFWContext &&other) noexcept {
    _init_options = other._init_options;
    _init_error = other._init_error;
    _window = other._window;
    _version = other._version;
    other._window = nullptr;
}

GLFWContext::~GLFWContext() {
    glfwTerminate();
}

#ifdef GIYGAS_WITH_OPENGL
void GLFWContext::initialize_for_opengl(GLVersion min, GLVersion max) {
    glfwWindowHint(GLFW_RESIZABLE, _init_options.is_resizable ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

    _version = max;
    while (_version >= min) {
        _window = try_create_opengl_window(
            _init_options.title,
            _init_options.width,
            _init_options.height,
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

    setup_callbacks();

    // Make the window's context current
    glfwMakeContextCurrent(_window);
}
#endif

#ifdef GIYGAS_WITH_VULKAN
bool GLFWContext::initialize_for_vulkan() {
    glfwWindowHint(GLFW_RESIZABLE, _init_options.is_resizable ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

    _window = glfwCreateWindow(
        _init_options.width,
        _init_options.height,
        _init_options.title,
        nullptr, // monitor
        nullptr  // share
    );

    if (_window == nullptr) {
        return false;
    }

    setup_callbacks();
    return true;
}
#endif

void GLFWContext::setup_callbacks() {
    glfwSetWindowUserPointer(_window, this);
    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
    glfwSetKeyCallback(_window, key_callback);
}

GLFWwindow *GLFWContext::try_create_opengl_window(
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

void GLFWContext::get_major_minor(GLVersion version, int &major, int &minor) {
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

void GLFWContext::set_init_options(GLFWWindowInitOptions options) {
    _init_options = options;
}

void GLFWContext::show() {
    glfwShowWindow(_window);
}

void GLFWContext::update() {
    glfwPollEvents();
}

#ifdef GIYGAS_WITH_OPENGL
void GLFWContext::present() {
    glfwSwapBuffers(_window);
}
#endif

bool GLFWContext::should_close() const {
    return static_cast<bool>(glfwWindowShouldClose(_window));
}

unsigned int GLFWContext::translate_key(InputKey key) const {
    return translate_to_glfw3_key(key);
}

float GLFWContext::get_input(uint32_t input) const {
    return glfwGetKey(_window, input);
}

EventHandler<uint32_t, float> GLFWContext::input_changed() {
    return _input_changed.make_handler();
}

bool GLFWContext::is_valid() const {
    return _window != nullptr;
}

void* GLFWContext::cast_to_specific(RendererType type) {
    // Returns a pointer that can safely be re-interpreted to the a pointer of
    // the given context type. (accounts for thunking)
#ifdef GIYGAS_WITH_OPENGL
    if (type == RendererType::OpenGL) {
        return static_cast<GLContext *>(this);
    }
#endif
#ifdef GIYGAS_WITH_VULKAN
    if (type == RendererType::Vulkan) {
        return static_cast<VulkanContext *>(this);
    }
#endif
    return nullptr;
}

#ifdef GIYGAS_WITH_OPENGL
void GLFWContext::make_current_on_calling_thread() {
    glfwMakeContextCurrent(_window);
}
#endif

#ifdef GIYGAS_WITH_VULKAN
const char** GLFWContext::get_required_instance_extensions(
    unsigned int *count
) const {
    return glfwGetRequiredInstanceExtensions(count);
}

VkResult GLFWContext::create_surface(
    VkInstance instance,
    VkSurfaceKHR *surface
) {
    return glfwCreateWindowSurface(instance, _window, nullptr, surface);
}
#endif //GIYGAS_WITH_VULKAN


void GLFWContext::framebuffer_size_callback(
    GLFWwindow *window,
    int width,
    int height
) {
    void *userdata = glfwGetWindowUserPointer(window);
    auto *instance = reinterpret_cast<GLFWContext *>(userdata);
//    instance->_framebuffer_width = static_cast<unsigned int>(width);
//    instance->_framebuffer_height = static_cast<unsigned int>(height);
//    instance->_surface_size_changed_event.invoke(
//        instance->_framebuffer_width,
//        instance->_framebuffer_height
//    );
}

void GLFWContext::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    void *userdata = glfwGetWindowUserPointer(window);
    auto *instance = reinterpret_cast<GLFWContext *>(userdata);

    // TODO: Support repeating keys through some interface. This will be useful for in-game
    // text input.
    if (action == GLFW_REPEAT) {
        return;
    }
    float val = action == GLFW_PRESS ? 1.0f : -1.0f;
    instance->_input_changed.invoke(static_cast<unsigned int>(key), val);
}