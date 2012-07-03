#include "dtach.h"

int
_log(const char *fmt, ...)
{
  char buffer[1024];
  va_list list;

  va_start(list, fmt);
  vsnprintf(buffer, 1204, fmt, list);
  fprintf(stderr, buffer);
  syslog (LOG_INFO, buffer);
  return 0;
}
