#ifndef ENGINE_LOGGING_H
#define ENGINE_LOGGING_H

#include <engine/interface.h>
#include <string>

/** 
 * @code
 * // logging example
 * Logging *log = interfaces.requestInterface<Logging>();
 * log->write(Logging::WARNING, "Something is wrong (%s)!", "argument");
 * @endcode
 */
class Logging : public Interface {
public:
  static std::string id() {
    return "logging";
  }

  enum LogType {
    TWEET = 0,
    ERROR = 1,
    WARNING = 2,
    DEBUG = 3
  };

  virtual void write(LogType, const char *format, ...) =0;
}; 

#endif // !ENGINE_LOGGING_H 

