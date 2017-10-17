#pragma once
#include <glad/glad.h>
#include <giygas/Texture.hpp>
#include <giygas_internal/GL.hpp>

namespace giygas {
    class GLTexture : public Texture {
        GL *_gl;
        GLuint _handle;

        static GLenum get_gl_texture_format(TextureFormat format);
        static size_t pixel_size_for_format(TextureFormat format);

    public:
        GLTexture(GL *gl);
        GLTexture(const GLTexture &) = delete;
        GLTexture(GLTexture &&) noexcept;
        GLTexture &operator=(const GLTexture &) = delete;
        GLTexture &operator=(GLTexture &&) noexcept;
        virtual ~GLTexture();

        RendererType renderer_type() const override;

        void set_data(
            const unsigned char* data, size_t size, size_t width, size_t height,
            TextureFormat format
        ) override;

        GLuint handle() const;

    };
}
