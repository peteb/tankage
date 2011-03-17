#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#include <sstream>
#include <vector>
#include <string>


struct log {
  enum log_level {
    error,
	warning,
    info,
	debug
  };
  struct log_consumer {
    virtual void write(log_level level, const std::string &line) =0;
    virtual ~log_consumer() {};
  };
  explicit log(log_level level);
  ~log();

  template<typename T> 
  log& operator<< (T data) {
    _stream << data;
	return *this;
  }
  static void register_consumer(log_consumer* consumer);

private:
  static std::vector<log::log_consumer*> _consumers;
  log_level _level;
  std::stringstream _stream;
};

#endif // !UTILS_LOG_H

