#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum
{
  LL_DEBUG,
  LL_INFO,
  LL_WARNING,
  LL_ERROR,
  LL_FATAL,
} LogLevel;

void _log(LogLevel level, const char *fmt, ...);
