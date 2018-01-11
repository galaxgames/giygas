#pragma once
#include <glad/glad.h>
#include <giygas/TextureFormat.hpp>

namespace giygas {
    GLenum get_gl_texture_format(TextureFormat format);
}
