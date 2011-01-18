#ifndef UTILS_ALGORITHM_H
#define UTILS_ALGORITHM_H


template<typename IterT, typename MemPredT, typename ParamT>
IterT remove_nif(IterT beg,
                 IterT end,
                 MemPredT pred,
                 ParamT &param) {
  IterT it = std::remove_if(
    beg,
    end,
    std::not1(std::bind2nd(std::mem_fun(pred), param)));
  
  return it;        
}

#endif //!UTILS_ALGORITHM_H

