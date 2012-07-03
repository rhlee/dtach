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
valid_master(struct master *master)
{
  if(master->pid == 0)
  {
    memset(master, 0, sizeof(struct master));
    return 0;
  }
  else if(kill(master->pid, 0) == -1)
  {
    if(errno == ESRCH)
    {
      memset(master, 0, sizeof(struct master));
      return 0;
    }
    else error(__LINE__, __FILE__);
  }
  return 1;
}

int
query_main()
{
  int i;
  struct master *master = load_masters();

  for(i = 0; i < MAX_MASTERS; i++)
  {
    valid_master(master);
    printf("%i %s\n", master->pid, master->address);
    master++;
  }
  
  return 0;
}

void register_master()
{
  char *path;
  struct master *master = load_masters();
  
  if((path = realpath(sockname, NULL)) == NULL)
    error(__LINE__, __FILE__);
  
  _log("socket: %s\n", path);

  while(valid_master(master)) master++;

  master->pid = getpid();
  strncpy(master->address, path, 256);
}
