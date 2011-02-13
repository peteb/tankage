#ifndef GAME_COMMON_NET_ERROR_H
#define GAME_COMMON_NET_ERROR_H

#include <stdexcept>
#include <string>
#include "net_protocol.h"

class NetError : public std::exception {
public:
  NetError(NetErrorCode code, const std::string &description);
  ~NetError() throw() {}
  
  const char *what() const throw();
  void fill(NetErrorMsg &msg) const;
  
private:
  const NetErrorCode _code;
  const std::string _desc;
  std::string _strRep;
};

#endif // !GAME_COMMON_NET_ERROR_H

