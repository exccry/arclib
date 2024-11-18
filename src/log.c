#include "log.h"

static const char *level_strings[] = {
  "DEBUG",
  "INFO",
  "WARNING",
  "ERROR",
  "FATAL"
};

void _log(LogLevel level, const char *fmt, ...)
{
  if (level < LL_INFO)
  {
    // handle debug
    return;
  }
 
  va_list args;
  va_start(args, fmt);


  fprintf(stderr, "[+ LOG +]--->[%s] ", level_strings[level]);
  vfprintf(stderr, fmt, args);
  fprintf(stderr, "\n");

  va_end(args);
}
