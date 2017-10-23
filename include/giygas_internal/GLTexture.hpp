#pragma once
#include <glad/glad.h>
#include <giygas/Texture.hpp>
#include <giygas_internal/GL.hpp>
#include <giygas/TextureInitOptions.hpp>

namespace giygas {
    class GLTexture : public Texture {
        GL *_gl;
        GLuint _handle;

        static size_t pixel_size_for_format(TextureFormat format);
        static GLenum enum_for_wrap_mode(TextureWrapMode mode);
        static GLenum enum_for_minify_filter_mode(TextureMinifyFilterMode mode);
        static GLenum enum_for_magnify_filter_mode(TextureMagnifyFilterMode mode);
    public:
        GLTexture(GL *gl, TextureInitOptions options);
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

        void create_storage(
            size_t width,
            size_t height,
            TextureFormat format
        ) override;

        void set_wrap_mode_u(TextureWrapMode mode) override;
        void set_wrap_mode_v(TextureWrapMode mode) override;
        void set_minify_filter_mode(TextureMinifyFilterMode mode) override;
        void set_magnify_filter_mode(TextureMagnifyFilterMode mode) override;

        GLuint handle() const;

    };
}
