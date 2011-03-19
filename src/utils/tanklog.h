
#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#include <sstream>
#include <vector>
#include <string>
#include <tr1/functional>

#define zerror tankage::tanklog::severity_error, __FILE__, __FUNCTION__, __LINE__
#define strange tankage::tanklog::severity_warning, __FILE__, __FUNCTION__, __LINE__
#define entertain tankage::tanklog::severity_info, __FILE__, __FUNCTION__, __LINE__
#define newermind tankage::tanklog::severity_debug, __FILE__, __FUNCTION__, __LINE__


namespace tankage {
class tanklog {
public:
  enum severity_t {
    severity_error,
    severity_warning,
    severity_info,
    severity_debug
  };

  tanklog(severity_t severity, const char* file, const char* function, int line);
  ~tanklog();

  template<typename T> 
  tanklog& operator<< (T data) {
    _stream << data;
    return *this;
  }

  typedef std::tr1::function<void(severity_t,const std::string&)> consumer_t;
  static void register_consumer(const consumer_t &consumer);

private:
  static std::vector<consumer_t> _consumers; 
  severity_t _severity;
  std::stringstream _stream;
};
} // !namespace

typedef tankage::tanklog tanklog;

#endif // !UTILS_LOG_H

