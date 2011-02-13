#include <game/common/net_error.h>
#include <sstream>
#include <cstring>

namespace {
std::string ErrorCodeToString(NetErrorCode code) {
  std::string ret;

  switch (code) {
  case NET_IDENT_WRONG_VERSION:
    ret = "wrong version";
    break;

  default:
    ret = "unknown error";
  }

  return ret;
}

}

NetError::NetError(NetErrorCode code, const std::string &description)
  : _code(code)
  , _desc(description)
{
  std::stringstream ss;
  ss << "Network error " << static_cast<int>(code) <<
    "(" << ErrorCodeToString(code) << "): " << description;

  _strRep = ss.str();
}
  
const char *NetError::what() const throw() {
  return _strRep.c_str();
}

void NetError::fill(NetErrorMsg &msg) const {
  msg.type = NET_ERROR;
  msg.error_code = _code;
  strncpy(msg.desc, _desc.c_str(), MAX_ERRDESC);
  msg.desc[MAX_ERRDESC-1] = '\0';
}
