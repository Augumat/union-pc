#pragma once
#include <istream>
#include <ostream>
#include <vector>

namespace unn
{
using u32 = uint32_t;
using s32 = int32_t;

class Reader
{
   public:
    Reader(std::istream &ifs);
    void read(char *buffer, size_t n);

   private:
    std::istream &ifs;
};

class Writer
{
   public:
    Writer(std::ostream &ofs);
    void write(const char *buffer, size_t n);

   private:
    std::ostream &ofs;
};

Writer &operator<<(Writer &w, u32 n);
Reader &operator>>(Reader &r, u32 &n);

Writer &operator<<(Writer &w, size_t n);
Reader &operator>>(Reader &r, size_t &n);

Writer &operator<<(Writer &w, int n);
Reader &operator>>(Reader &r, int &n);

template <typename T>
Writer &operator<<(Writer &w, const std::vector<T> &vec);
template <typename T>
Reader &operator>>(Reader &w, std::vector<T> &vec);
Reader &operator>>(Reader &r, std::string &s);
Writer &operator<<(Writer &w, const std::string &s);
}  // namespace unn

#include "serialization.inl"
