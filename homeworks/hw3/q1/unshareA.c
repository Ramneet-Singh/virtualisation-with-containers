#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 1024

int main(int argc, char* argv[])
{
  int ret = unshare(CLONE_NEWUTS);
  if (ret==-1)
  {
    perror("unshare");
    exit(EXIT_FAILURE);
  }
  const char *newHostName = "maverick-new";
  ret = sethostname(newHostName, 12);
  if (ret==-1)
  {
    perror("sethostname");
    exit(EXIT_FAILURE);
  }
  
  char hostName[MAX_LEN];
  ret = gethostname(hostName, MAX_LEN);
  if (ret==-1)
  {
    perror("gethostname");
    exit(EXIT_FAILURE);
  }
  printf("My hostname is: \"%s\"\n", hostName);

  for( ; ; ) {}
  return 0;
}
