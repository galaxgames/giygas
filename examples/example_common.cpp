#include "example_common.hpp"
#include <giygasutil/paths.hpp>
#include <fstream>
#include <algorithm>

using namespace giygas_examples_common;
using namespace giygasutil;

ShaderLoader::ShaderLoader(const char *arg0, RendererType renderer_type) {
    _content_dir = paths::basename(arg0);
    paths::append(_content_dir, "content");
    _renderer_type = renderer_type;
}

unique_ptr<uint8_t[]> ShaderLoader::get_shader_code(const char *name, size_t &size) const {
    string path = _content_dir;
    paths::append(path, name);
    const char *postfix;
    if (_renderer_type == RendererType::OpenGL) {
        postfix = ".glsl";
    }
    else {
        postfix = ".spv";
    }
    path.append(postfix);

    unique_ptr<uint8_t[]> data;
    size_t data_size = 0;
    const int buffer_len = 256;
    unique_ptr<char[]> buffer(new char[buffer_len]);

    ifstream stream(path, ifstream::binary);
    while (!stream.eof() && stream.good()) {
        size_t previous_data_size = data_size;
        stream.read(buffer.get(), buffer_len);
        streamsize chars_read = stream.gcount();

        // Resize data
        data_size = previous_data_size + chars_read;
        unique_ptr<uint8_t[]> new_data(new uint8_t[data_size]);
        copy_n(data.get(), previous_data_size, new_data.get());
        data = move(new_data);

        // Put read data
        for (streamsize i = 0; i < chars_read; ++i) {
            data[previous_data_size + i] = static_cast<uint8_t>(buffer[i]);
        }
    }

    size = data_size;
    return data;
}