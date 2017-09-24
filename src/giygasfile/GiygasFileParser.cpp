#include <giygasfile/GiygasFileParser.hpp>

using namespace giygasfile;
using namespace std;

GiygasFileParser::GiygasFileParser(InputIterable *input) {
    _input = input;
}

GiygasData GiygasFileParser::parse() {
    _vbo_nodes.clear();
    _ebo_nodes.clear();
    _vao_nodes.clear();

    parse_header();
    NodeTag tag;
    while (_input->read(sizeof(tag), &tag) > 0) {
        parse_node(tag);
    }


    // Build the data
    GiygasData data;
    data.vbo_count = _vbo_nodes.size();
    data.vbos = std::unique_ptr<VBONode[]>(new VBONode[data.vbo_count]);
    for (size_t i = 0; i < data.vbo_count; ++i) {
        data.vbos[i] = std::move(_vbo_nodes[i]);
    }
    data.ebo_count = _ebo_nodes.size();
    data.ebos = std::unique_ptr<EBONode[]>(new EBONode[data.ebo_count]);
    for (size_t i = 0; i < data.ebo_count; ++i) {
        data.ebos[i] = std::move(_ebo_nodes[i]);
    }
    data.vao_count = _vao_nodes.size();
    data.vaos = std::unique_ptr<VAONode[]>(new VAONode[data.vao_count]);
    for (size_t i = 0; i < data.vao_count; ++i) {
        data.vaos[i] = std::move(_vao_nodes[i]);
    }
    return data;
}

const HeaderData &GiygasFileParser::header() const {
    return _header;
}

void GiygasFileParser::parse_header() {
    _input->read(sizeof(HeaderData), &_header);
}

void GiygasFileParser::parse_node(NodeTag tag) {
    switch (tag) {
    case NodeTag::VBONode:
        parse_vbo_node();
        break;
    case NodeTag::EBONode:
        parse_ebo_node();
        break;
    case NodeTag::VAONode:
        parse_vao_node();
        break;
    }
}

// TODO: Is something like this defined in the standard library?
// Because that would be pretty nice!!
template<typename T>
class TypeWrapper {
public:
    typedef T type;
};

#define READ_FIELD(input, T, field) { \
    static_assert(std::is_object<T>::value, "T must be an object"); \
    typedef decltype(field) field_t; \
    static_assert(std::is_same< \
        std::conditional< \
            std::is_enum<field_t>::value, \
            std::underlying_type<field_t>, TypeWrapper<field_t> \
        >::type::type, T \
    >::value, "type mismatch"); \
    input->read(sizeof(field), &field); \
}

void GiygasFileParser::parse_vbo_node() {
    VBONode node;
    READ_FIELD(_input, unsigned int, node.size);
    node.data = unique_ptr<unsigned char[]>(new unsigned char[node.size]);
    _input->read(node.size, node.data.get());
    _vbo_nodes.emplace_back(std::move(node));
}

void GiygasFileParser::parse_ebo_node() {
    EBONode node;
    READ_FIELD(_input, unsigned int, node.count);
    READ_FIELD(_input, unsigned int, node.element_size);
    size_t size = node.count * node.element_size;
    node.data = unique_ptr<unsigned char[]>(new unsigned char[size]);
    _input->read(size, node.data.get());
    _ebo_nodes.emplace_back(std::move(node));
}

void GiygasFileParser::parse_vao_node() {
    typedef unsigned short int uint16;
    typedef unsigned char uint8;
    VAONode node;
    READ_FIELD(_input, uint16, node.buffer_count);
    node.buffers = unique_ptr<VAOBufferData[]>(new VAOBufferData[node.buffer_count]);

    for (size_t i = 0; i < node.buffer_count; ++i) {
        VAOBufferData buffer;
        READ_FIELD(_input, uint16, buffer.attribute_count);
        READ_FIELD(_input, uint16, buffer.attribute_stride);
        buffer.attributes = unique_ptr<AttributeData[]>(new AttributeData[buffer.attribute_count]);

        for (uint16 j = 0; j < buffer.attribute_count; ++j) {
            AttributeData attrib;
            READ_FIELD(_input, uint8, attrib.component_size);
            READ_FIELD(_input, uint8, attrib.component_count);
            READ_FIELD(_input, uint8, attrib.component_offset);
            READ_FIELD(_input, uint8, attrib.hint);
            buffer.attributes[j] = std::move(attrib);
        }
        node.buffers[i] = std::move(buffer);
    }

    _vao_nodes.emplace_back(std::move(node));
}
