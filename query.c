#include "dtach.h"

struct master
{
  int pid;
  char address[256];
  int clients;
};

struct master
*load_masters()
{
  int shm_fd;
  struct master *masters;
  
  if((shm_fd = shm_open("dtach", O_CREAT | O_RDWR, 0600)) == -1)
    error(__LINE__, __FILE__);

  if(ftruncate(shm_fd, MAX_MASTERS * sizeof(struct master)) == -1)
    error(__LINE__, __FILE__);

  if((masters = mmap(NULL, MAX_MASTERS * sizeof(struct master),
      PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0)) ==
    MAP_FAILED)
    error(__LINE__, __FILE__);
    
  return masters;
}

int
query_main()
{
  struct master *masters= load_masters();
  return 0;
}

void register_master()
{
  char *path;
  struct master *masters= load_masters();
  
  if((path = realpath(sockname, NULL)) == NULL)
    error(__LINE__, __FILE__);
  
  _log("socket: %s\n", path);
}
