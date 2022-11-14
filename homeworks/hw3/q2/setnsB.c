#define _GNU_SOURCE
#include <sched.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_LEN 1024

int main(int argc, char *argv[])
{
  // Arg1 - PID Arg2 - Command
  // Open /proc/PID/ns/uts
  char pathname[1024];
  strcpy(pathname, "/proc/");
  strcat(pathname, argv[1]);
  strcat(pathname, "/ns/uts");
  int fd = open(pathname, O_RDONLY | O_CLOEXEC);
  if (fd==-1)
  {
    perror("open");
    exit(EXIT_FAILURE);
  }

  // Join the namespace
  int ret = setns(fd, CLONE_NEWUTS);
  if (ret==-1)
  {
    perror("setns");
    exit(EXIT_FAILURE);
  }

  // Run the command
  execlp(argv[2], basename(argv[2]), (char *) NULL);

  // Shouldn't reach here
  perror("execlp");
  exit(EXIT_FAILURE);
  return 0;
}
