#ifndef UTILS_KEY_H
#define UTILS_KEY_H

struct key_cmp {
  template<typename KeyT>
  bool operator() (const KeyT &lhs, const KeyT &rhs) {
    return lhs.first < rhs.first;
  }

  template<typename KeyT>
  bool operator() (const KeyT &lhs, const typename KeyT::first_type &rhs) {
    return lhs.first < rhs;
  }

  template<typename KeyT>
  bool operator() (const typename KeyT::first_type &lhs, const KeyT &rhs) {
    return lhs < rhs.first;
  }
};

#endif // !UTILS_PAIRCMP_H
