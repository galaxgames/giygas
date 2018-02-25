#include "example_common.hpp"
#include <giygasutil/paths.hpp>
#include <fstream>
#include <algorithm>
#include <cassert>

using namespace giygas_examples_common;
using namespace giygasutil;

ShaderLoader::ShaderLoader(const char *contentDir, RendererType renderer_type) {
    _content_dir = string(contentDir);
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


#pragma pack(1)
class TargaColorMapSpec {
public:
    uint16_t origin;
    uint16_t length;
    uint8_t entry_size;
};

#pragma pack(1)
class TargaImageSpec {
public:
    uint16_t origin_x;
    uint16_t origin_y;
    uint16_t width;
    uint16_t height;
    uint8_t bits_per_pixel;
    uint8_t flags;
};

#pragma pack(1)
class TargaHeader {
public:
    uint8_t id_size;
    uint8_t color_map_type;
    uint8_t image_type;
    TargaColorMapSpec color_map_spec;
    TargaImageSpec image_spec;
};


string giygas_examples_common::get_content_dir(const char *arg0) {
    string content_dir = paths::basename(arg0);
    paths::append(content_dir, "content");
    return content_dir;
}

uint8_t * giygas_examples_common::load_targa(
    istream &input,
    size_t &size,
    uint8_t &bytes_per_pixel_out,
    uint32_t &width,
    uint32_t &height
) {
    TargaHeader header = {};
    input.read(reinterpret_cast<char *>(&header), sizeof(header));

    assert(header.id_size == 0); // we so dumb
    assert(header.color_map_type == 0); // We can't handle color mapped targas cuz we're dumb.
    assert(header.image_type == 2); // We only support unmapped images, cuz again, we're dumb.
    assert(header.image_spec.bits_per_pixel == 24 || header.image_spec.bits_per_pixel == 32); // dumb

    assert(header.image_spec.width < 2048 && header.image_spec.height < 2048);  // dumb dumb dumb!
    size_t bytes_per_pixel = static_cast<size_t>(header.image_spec.bits_per_pixel / 8);
    size_t image_size = header.image_spec.width * header.image_spec.height * bytes_per_pixel;
    uint8_t *image_data = new uint8_t[image_size];

    input.read(reinterpret_cast<char *>(image_data), image_size);

    // bgr -> rgb
    for (int i = 0; i < image_size; i += bytes_per_pixel) {
        uint8_t r = image_data[i + 2];
        uint8_t b = image_data[i];
        image_data[i] = r;
        image_data[i + 2] = b;
    }

    size = image_size;
    bytes_per_pixel_out = static_cast<uint8_t>(bytes_per_pixel);
    width = header.image_spec.width;
    height = header.image_spec.height;
    return image_data;
}