
#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#include <sstream>
#include <vector>
#include <string>
#include <tr1/functional>

#define error tankage::log::severity_error, __FILE__, __FUNCTION__, __LINE__
#define warning tankage::log::severity_warning, __FILE__, __FUNCTION__, __LINE__
#define info tankage::log::severity_info, __FILE__, __FUNCTION__, __LINE__
#define debug tankage::log::severity_debug, __FILE__, __FUNCTION__, __LINE__


namespace tankage {

class log {
public:
  enum severity_t {
    severity_error,
    severity_warning,
    severity_info,
    severity_debug
  };

  log(severity_t severity, const char* file, const char* function, int line);
  ~log();

  template<typename T> 
  log& operator<< (T data) {
    _stream << data;
    return *this;
  }

  typedef std::tr1::function<void(severity_t,const std::string&)> log_consumer_t;
  static void register_consumer(const log_consumer_t &consumer);

private:
  static std::vector<log_consumer_t> _consumers; 
  severity_t _severity;
  std::stringstream _stream;
};
} // !namespace

typedef tankage::log tanklog;

#endif // !UTILS_LOG_H

