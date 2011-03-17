#ifndef UTILS_LOG_H
#define UTILS_LOG_H

// these will go out, left for inline implementation
// to check how stuff look like
#include <iostream>
#include <string.h>
#include <cstdlib>
#include <sstream>


// suggest keep class to keep it as namespace and maybe later use some
// common static variable like stream where to print or somthing
struct log {
  enum log_level {
    error,
	warning,
    info,
	debug
  };
  // constructor with level
  log(const log_level level) : _level(level) {}
  // printer operator
  template<typename T> 
  log& operator<< (T text) {
	// collects all the stuff during the lifecycle
    _stream << text;
	return *this;
  }
  ~log() {
    // dumps all the log at dying, that means it's reached the end
    std::cout << "level: " << _level << ", text: " << _stream.str() << std::endl;
  }

private:
  log_level _level;
  std::stringstream _stream;
};


#endif // !UTILS_LOG_H

