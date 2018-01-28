#include <giygas/GLFWWindowInitOptions.hpp>

using namespace giygas;

//const GLVersion DEFAULT_GL_MIN_VERSION = GLVersion::GL_3_2;
//const GLVersion DEFAULT_GL_MAX_VERSION = GLVersion::GL_4_6;
const char *DEFAULT_WINDOW_TITLE = "Giygas GLFW Window";

GLFWWindowInitOptions::GLFWWindowInitOptions() {
    is_resizable = true;
    title = DEFAULT_WINDOW_TITLE;
    width = 640;
    height = 480;
}

GLFWWindowInitOptions::GLFWWindowInitOptions(
    bool is_resizable,
    const char *title,
    unsigned int width,
    unsigned int height
)
    : is_resizable(is_resizable)
    , title(title)
    , width(width)
    , height(height)
{}
