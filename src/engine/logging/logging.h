#ifndef ENGINE_LOGGING_LOGGING_H
#define ENGINE_LOGGING_LOGGING_H

#include <engine/logging.h>
#include <utils/singleton.h>

namespace Log {
class Logging : public ::Logging, public Singleton<Logging> {
public:
  Logging() {};
  
  void write(LogType type, const char *format, ...);
};
}

#endif // !ENGINE_LOGGING_LOGGING_H

