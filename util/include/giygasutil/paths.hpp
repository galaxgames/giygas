#pragma once
#include <string>

namespace giygasutil {
    namespace paths {
        using namespace std;

        string normalize(const char *path);
        string basename(const char *path);
        void append(string &path, const char *part);

    }
}