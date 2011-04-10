#ifndef PLATFORM_LOGGING_LOGGING_H
#define PLATFORM_LOGGING_LOGGING_H

#include <twitcurl/twitcurl.h>
#include <platform/logging.h>
#include <utils/singleton.h>

namespace Log {
class Logging : public ::Logging, public Singleton<Logging> {
public:
  Logging(std::ostream &output = std::cout);
  
  void write(LogType type, const char *format, ...);

private:
  twitCurl _twitter;
  size_t _level; 
  std::ostream &_output; 
};
}

#endif // !PLATFORM_LOGGING_LOGGING_H

