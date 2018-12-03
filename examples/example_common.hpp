#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include <giygas/RendererType.hpp>

namespace giygas_examples_common {

    class ShaderLoader {
        std::string _content_dir;
        giygas::RendererType _renderer_type;

    public:
        ShaderLoader() = default;
        ShaderLoader(const char *contentDir, giygas::RendererType renderer_type);
        std::unique_ptr<uint8_t[]> get_shader_code(const char *name, size_t &size) const;
    };

    std::string get_content_dir(const char *arg0);

    uint8_t *load_targa(
        std::istream &input,
        size_t &size,
        uint8_t &bytes_per_pixel,
        uint32_t &width,
        uint32_t &height
    );

}
