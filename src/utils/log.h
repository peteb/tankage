
#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#include <sstream>
#include <vector>
#include <string>

#define error tankage::log::severity_error, __FILE__, __FUNCTION__, __LINE__
#define warning tankage::log::severity_warning, __FILE__, __FUNCTION__, __LINE__
#define info tankage::log::severity_info, __FILE__, __FUNCTION__, __LINE__
#define debug tankage::log::severity_debug, __FILE__, __FUNCTION__, __LINE__


namespace tankage {
struct log {
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

  struct log_consumer {
    virtual void write(severity_t severity, const std::string &line) =0;
    virtual ~log_consumer() {};
  };
  static void register_consumer(log_consumer* consumer);

private:
  static std::vector<log::log_consumer*> _consumers;
  severity_t _severity;
  std::stringstream _stream;
};
} // !namespace

typedef tankage::log tlog;

#endif // !UTILS_LOG_H

