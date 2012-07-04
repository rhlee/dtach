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
  int i, valid_masters = 0;
  struct master *master = load_masters();
  size_t size = 8192;
  char buffer[size];
  buffer[0] = 0;
  
  for(i = 0; i < MAX_MASTERS; i++)
  {
    if(valid_master(master))
    {
      size -= snprintf(buffer, size, "%i %i %s\n",
        master->pid, master->clients, master->address);
      valid_masters = 1;
    }
    master++;
  }

  if(valid_masters) printf(buffer);
  else printf("No current dtach sessions");
  
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

void register_client()
{
  struct master *master = load_masters();
  master->clients++;
}

void deregister_client()
{
  struct master *master = load_masters();
  master->clients--;
}
