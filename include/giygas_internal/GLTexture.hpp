#pragma once
#include <glad/glad.h>
#include <giygas/Texture.hpp>
#include <giygas/GL.hpp>

namespace giygas {
    class GLTexture : public Texture {
        GL *_gl;
        GLuint _handle;

        static GLenum get_gl_texture_format(TextureFormat format);

    public:
        GLTexture(GL *gl);
        GLTexture(const GLTexture &) = delete;
        GLTexture(GLTexture &&) noexcept;
        GLTexture &operator=(const GLTexture &) = delete;
        GLTexture &operator=(GLTexture &&) noexcept;
        virtual ~GLTexture();

        void set_data(
            const char* data, int size, int width, int height,
            TextureFormat format
        ) override;

        GLuint get_handle() const;

    };
}