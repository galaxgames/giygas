#pragma once
#include <string>

namespace giygasutil {
namespace paths {

    std::string normalize(const char *path);
    std::string basename(const char *path);
    void append(std::string &path, const char *part);

}}
