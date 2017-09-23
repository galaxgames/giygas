#include <giygasfile/FileIterable.hpp>

using namespace std;
using namespace giygasfile;

FileIterable::FileIterable(const string &filename) :
    _file(filename, ios::binary | ios::in)
{
}

size_t FileIterable::read(size_t count, void *out) {
    _file.read(reinterpret_cast<char *>(out), static_cast<streamsize>(count));
    return static_cast<size_t>(_file.gcount());
}
