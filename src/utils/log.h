#ifndef UTILS_LOG_H
#define UTILS_LOG_H

// these will go out, left for inline implementation
// to check how stuff look like
#include <iostream>
#include <string.h>
#include <cstdlib>


// suggest keep class to keep it as namespace and maybe later use some
// common static variable like stream where to print or somthing
struct log {
  // i think we should keep levels to be able to filter
  enum log_level {
    error,
	warning,
    info,
	debug
  };
  // main write
  static void write(log_level type, const char *format, ...) {
    std::cout << format << std::endl;
  }
  // tweet probably should be seperate function like this
  static void tweet(const char* format, ...) {
    // inform in log and do tweet
	write(info, format);
  }
  // scoped enter, does this make sense? just came in mind?
  struct scoped_enter {
    scoped_enter(const char* something = "") : _something(something) {
      write(debug, std::string("<< ").append(_something).c_str());
    }
    ~scoped_enter() {
      write(debug,  std::string(">> ").append(_something).c_str());
    }
  private:
    std::string _something;
};
private:
  log() {}
};



#endif // !UTILS_LOG_H

