#pragma once
#include <memory>
#include <giygasfile/export.h>
#include <giygasfile/AttributeUsageHint.hpp>

namespace giygasfile {

    class GIYGASFILE_EXPORT HeaderData {
    public:
        char magic[8];
        unsigned int version;
    };

    class GIYGASFILE_EXPORT VBONode {
    public:
        std::unique_ptr<unsigned char[]> data;
        unsigned int size;
    };

    class GIYGASFILE_EXPORT EBONode {
    public:
        std::unique_ptr<unsigned char[]> data;
        unsigned int count;
        unsigned int element_size;
    };

    class GIYGASFILE_EXPORT AttributeData {
    public:
        unsigned char component_size;
        unsigned char component_count;
        unsigned char component_offset;
        AttributeUsageHint hint;
    };

    class GIYGASFILE_EXPORT VAOBufferData {
    public:
        std::unique_ptr<AttributeData[]> attributes;
        unsigned short int attribute_count;
        unsigned short int attribute_stride;
    };

    class GIYGASFILE_EXPORT VAONode {
    public:
        std::unique_ptr<VAOBufferData[]> buffers;
        unsigned short int buffer_count;
    };

    class GIYGASFILE_EXPORT GiygasData {
      public:
        std::unique_ptr<VBONode[]> vbos;
        std::unique_ptr<EBONode[]> ebos;
        std::unique_ptr<VAONode[]> vaos;
        size_t vbo_count;
        size_t ebo_count;
        size_t vao_count;
    };
}
