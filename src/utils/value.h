#ifndef UTILS_VALUE_H
#define UTILS_VALUE_H

#include <algorithm>

template<typename ValueT>
ValueT clamp(const ValueT &val, const ValueT &min, const ValueT &max) {
  return std::max(std::min(val, max), min);
}

#endif // !UTILS_VALUE_H
