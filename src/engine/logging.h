#ifndef ENGINE_LOGGING_H
#define ENGINE_LOGGING_H

#include <engine/interface.h>
#include <string>


/** 
 * @code
 * // a log example
 * Logging *log = interfaces.requestInterface<Logging>();
 * log->write(WARNING, "Something bad happened (%s)", argument);
 * @endcode
 */
class Logging : public Interface {
public:
  static std::string id() {
    return "logging";
  }

  enum LogType {
    ERROR = 0,
    WARNING = 1,
    DEBUG = 2
  };

  virtual void write(LogType, const char* format, ...) =0;
}; 

#endif // !ENGINE_LOGGING_H 

