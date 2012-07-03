#include "dtach.h"

void
flatten(char * output, size_t size, char * input)
{
  size--;
  while(*input != '\0')
  {
    if(*input == '\n' && (*(input+1) != '\0'))
    {
      if(size > 3)
      {
        strcpy(output, " \\ ");
        output += 3;
        size -= 3;
      }
      else break;
    }
    else
      if(size > 0)
      {
        *(output++) = *input;
        size--;
      }
    input++;
  }
  *output = '\0';
}

int
_log(const char *fmt, ...)
{
  char buffer[1024], flattened[1024];
  va_list list;

  va_start(list, fmt);
  vsnprintf(buffer, 1024, fmt, list);
  fprintf(stderr, buffer);
  flatten(flattened, 1024, buffer);
  syslog (LOG_INFO, flattened);
  return 0;
}

void
error(int line, char * file)
{
  char buffer[1024];
  snprintf(buffer, 1024,
    "[%s:%i] Last set error code is %i: %s\nUse gdb to catch this SIGTRAP",
    file, line, errno, strerror(errno));
  _log(buffer);
  __asm__("int3");
  exit(errno);
}
