#pragma once
#include <giygasfile/export.h>
#include "InputIterable.hpp"
#include <string>
#include <fstream>

namespace giygasfile {
    class GIYGASFILE_EXPORT FileIterable : public InputIterable {
        std::ifstream _file;
    public:
        FileIterable(const std::string &filename);
        size_t read(size_t count, void *out) override;
    };
}
