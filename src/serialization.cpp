#include "unn/serialization.hpp"

namespace unn
{
Reader::Reader(std::istream &ifs) : ifs(ifs) {}

void Reader::read(char *buffer, size_t n) { ifs.read(buffer, n); }

Writer::Writer(std::ostream &ofs) : ofs(ofs) {}

void Writer::write(const char *buffer, size_t n) { ofs.write(buffer, n); }

Writer &operator<<(Writer &w, u32 n)
{
    char bytes[] = {
        (char)(n),
        (char)(n >> 8),
        (char)(n >> 16),
        (char)(n >> 24),
    };
    w.write(bytes, 4);
    return w;
}

Reader &operator>>(Reader &r, u32 &n)
{
    char data[4];
    r.read(data, 4);
    n = u32(data[0]) + u32(data[1] << 8) + u32(data[2] << 16) +
        u32(data[3] << 24);
    return r;
}

Writer &operator<<(Writer &w, size_t n)
{
    char bytes[] = {
        (char)(n),
        (char)(n >> 8),
        (char)(n >> 16),
        (char)(n >> 24),
    };
    w.write(bytes, 4);
    return w;
}

Reader &operator>>(Reader &r, size_t &n)
{
    char data[4];
    r.read(data, 4);
    n = size_t(data[0]) + size_t(data[1] << 8) + size_t(data[2] << 16) +
        size_t(data[3] << 24);
    return r;
}

Writer &operator<<(Writer &w, int n)
{
    char bytes[] = {
        (char)(n),
        (char)(n >> 8),
        (char)(n >> 16),
        (char)(n >> 24),
    };
    w.write(bytes, 4);
    return w;
}

Reader &operator>>(Reader &r, int &n)
{
    char data[4];
    r.read(data, 4);
    n = (int((unsigned char)data[0])) | (int((unsigned char)data[1]) << 8) |
        (int((unsigned char)data[2]) << 16) |
        (int((unsigned char)data[3]) << 24);
    return r;
}

Reader &operator>>(Reader &r, std::string &s)
{
    size_t size;
    r >> size;
    char *buffer = new char[size];
    r.read(buffer, size);
    s = std::string(buffer, buffer + size);
    delete[] buffer;
    return r;
}

Writer &operator<<(Writer &w, const std::string &s)
{
    w << s.size();
    w.write(s.c_str(), s.size());
    return w;
}
}  // namespace unn
