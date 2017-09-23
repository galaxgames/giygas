#pragma once
#include "GiygasData.hpp"
#include "InputIterable.hpp"
#include <vector>

namespace giygasfile {

    enum class NodeTag : int {
        VBONode = 0,
        EBONode = 1,
        VAONode = 2
    };

    class GiygasFileParser {
        InputIterable *_input;
        HeaderData _header;
        std::vector<VBONode> _vbo_nodes;
        std::vector<EBONode> _ebo_nodes;
        std::vector<VAONode> _vao_nodes;

        void parse_header();
        void parse_node(NodeTag tag);
        void parse_vbo_node();
        void parse_ebo_node();
        void parse_vao_node();

    public:
        GiygasFileParser(InputIterable *input);
        GiygasFileParser(const GiygasFileParser &) = delete;
        GiygasFileParser(GiygasFileParser &&) = default;
        GiygasFileParser &operator=(const GiygasFileParser &) = delete;
        GiygasFileParser &operator=(GiygasFileParser &&) = default;

        GiygasData parse();
        const HeaderData &header() const;
    };
}
