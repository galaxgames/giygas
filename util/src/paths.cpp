#include <giygasutil/paths.hpp>
#include <cstring>

using namespace giygasutil;
using namespace std;

string paths::normalize(const char *path) {
    string path_string = string(path);
    string::size_type i;
    while((i = path_string.find('\\')) != string::npos) {
        path_string.replace(i, 1, "/");
    }
    return path_string;
}

string paths::basename(const char *path) {
    size_t len = strlen(path);
    while(len > 0) {
        if (path[len - 1] == '/') {
            break;
        }
        --len;
    }
    if (len > 0) {
        // Exclude found seperator
        --len;
    }

    return string(path, len);
}

void paths::append(string &path, const char *part) {
    if (path[path.size() - 1] != '/') {
        path.push_back('/');
    }

    size_t part_len = strlen(part);
    size_t part_start = 0;
    while (part_start < part_len) {
        if (part[part_start] != '/') {
            break;
        }
        ++part_start;
    }

    size_t part_end = part_len;
    while (part_end > 0) {
        if (part[part_end - 1] != '/') {
            break;
        }
        --part_end;
    }

    path.append(part + part_start, part_end - part_start);
}