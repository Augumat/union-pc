#include "unn/serialization.hpp"

namespace unn
{
template <typename T>
Writer &operator<<(Writer &w, const std::vector<T> &vec)
{
    w << vec.size();
    for (auto &i : vec) {
        w << i;
    }
    return w;
}
template <typename T>
Reader &operator>>(Reader &w, std::vector<T> &vec)
{
    size_t size;
    w >> size;
    vec.resize(size);
    for (size_t i = 0; i < size; ++i) {
        w >> vec[i];
    }
    return w;
}

}  // namespace unn
