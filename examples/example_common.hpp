#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include <giygas/RendererType.hpp>


namespace giygas_examples_common {
    using namespace std;
    using namespace giygas;

    class ShaderLoader {
        string _content_dir;
        RendererType _renderer_type;

    public:
        ShaderLoader() = default;
        ShaderLoader(const char *arg0, RendererType renderer_type);
        unique_ptr<uint8_t[]> get_shader_code(const char *name, size_t &size) const;
    };



}