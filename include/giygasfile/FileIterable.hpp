#pragma once
#include "InputIterable.hpp"
#include <string>
#include <fstream>

namespace giygasfile {
    class FileIterable : public InputIterable {
        std::ifstream _file;
    public:
        FileIterable(const std::string &filename);
        size_t read(size_t count, void *out) override;
    };
}
