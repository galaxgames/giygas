#pragma once
#include <glad/glad.h>
#include <giygas/Texture.hpp>

namespace giygas {
    class GLTexture : public Texture {
        GLuint _handle;

    
        static GLenum get_gl_texture_format(TextureFormat format);

    public:
        GLTexture();
        GLTexture(const GLTexture &) = delete;
        GLTexture& operator=(const GLTexture &) = delete;
        GLTexture(GLTexture &&);
        virtual ~GLTexture();

        void set_data(
            const char* data, int size, int width, int height,
            TextureFormat format
        ) override;

        GLuint get_handle() const;

    };
}