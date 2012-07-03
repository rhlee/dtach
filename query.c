#include "dtach.h"

struct master
{
  int pid;
  char address[256];
  int clients;
};

int
query_main(char *path)
{
  
  return 0;
}

void register_master()
{
  char *path;
  
  if((path = realpath(sockname, NULL)) == NULL)
    error(__LINE__, __FILE__);
  
  _log("socket: %s\n", path);
}
