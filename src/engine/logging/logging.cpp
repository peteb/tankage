#include <engine/logging/logging.h>

#include <cstdio>
#include <cstdarg>

void Log::Logging::write(Logging::LogType type, const char *format, ...) {

  switch (type) {
  case Logging::ERROR:
    printf("ERROR: ");
    break;
  case Logging::WARNING:
    printf("WARNING: ");
    break;
  case Logging::DEBUG:
    printf("DEBUG: ");
    break;
  default:
    break;
  }

  va_list args;
  va_start (args, format);
  vprintf(format, args);
  va_end (args);
  printf("\n");
}

