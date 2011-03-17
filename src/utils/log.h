#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#include <iostream>
#include <sstream>
#include <list>
#include <string>


struct log {
  struct log_consumer {
    virtual void write(const std::string& line) =0;
    virtual ~log_consumer() {};
  };
  enum log_level {
    error,
	warning,
    info,
	debug
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
  static std::list<log::log_consumer*> _consumers;
  log_level _level;
  std::stringstream _stream;
};

#endif // !UTILS_LOG_H

