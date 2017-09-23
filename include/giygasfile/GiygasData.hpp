#pragma once
#include <memory>

namespace giygasfile {

    class HeaderData {
    public:
        char magic[8];
        unsigned int version;
    };

    class VBONode {
    public:
        std::unique_ptr<unsigned char[]> data;
        size_t size;
    };

    class EBONode {
    public:
        std::unique_ptr<unsigned char[]> data;
        size_t count;
        size_t element_size;
    };

    class AttributeData {
    public:
        unsigned char component_size;
        unsigned char component_count;
    };

    class VAOBufferData {
    public:
        std::unique_ptr<AttributeData[]> attributes;
        size_t attribute_count;
    };

    class VAONode {
    public:
        std::unique_ptr<VAOBufferData[]> buffers;
        size_t buffer_count;
    };

    class GiygasData {
      public:
        std::unique_ptr<VBONode[]> vbos;
        std::unique_ptr<EBONode[]> ebos;
        std::unique_ptr<VAONode[]> vaos;
        size_t vbo_count;
        size_t ebo_count;
        size_t vao_count;
    };
}
