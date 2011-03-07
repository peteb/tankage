#ifndef UTILS_ALGORITHM_H
#define UTILS_ALGORITHM_H

#include <algorithm>

template<typename IterT, typename MemPredT, typename ParamT>
IterT remove_nif(IterT beg,
                 IterT end,
                 MemPredT pred,
                 ParamT &param) {
  IterT it = std::partition(
    beg,
    end,
    std::bind2nd(std::mem_fun(pred), param));
  
  return it;        
}

struct delete_op {
  template<typename DataT>
  void operator()(DataT &data) {
    delete data;
  }
};

template<typename T, typename Scalar>
T lerp(const T &begin, const T &end, Scalar amount) {
  return begin + (begin - end) * amount;
}
#endif //!UTILS_ALGORITHM_H

