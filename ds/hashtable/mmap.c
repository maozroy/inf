#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

int main(void) 
{
  size_t pagesize = sysconf(_SC_PAGESIZE);
  char * region = NULL;
  int unmap_result = 0;
  printf("System page size: %ld bytes\n", pagesize);

  region = mmap((void*) (pagesize *(1 << 20)),   
    pagesize,                         
    PROT_READ|PROT_WRITE|PROT_EXEC,
    _PC_MAX_CANON|MAP_PRIVATE,           
    0,
    0
  );
  if (region == MAP_FAILED) {
    perror("Could not mmap");
    return 1;
  }

  strcpy(region, "Hello, world!");

  printf("Contents of region: %s\n", region);

  unmap_result = munmap(region, 1 << 10);
  if (unmap_result != 0) {
    perror("Could not munmap");
    return 1;
  }

  return 0;
}

