#include <engine/logging/logging.h>

#include <cstdio>
#include <cstdarg>

void Log::Logging::write(Logging::LogType type, const char *format, ...) {

  switch (type) {
    case Logging::ERROR:
      printf("ER ");
      break;
    case Logging::WARNING:
      printf("WA ");
      break;
    case Logging::DEBUG:
      printf("DE ");
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

