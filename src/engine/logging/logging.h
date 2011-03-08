#ifndef ENGINE_LOGGING_LOGGING_H
#define ENGINE_LOGGING_LOGGING_H

#include <twitcurl/twitcurl.h>
#include <engine/logging.h>
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

#endif // !ENGINE_LOGGING_LOGGING_H

