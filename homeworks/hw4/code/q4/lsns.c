#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                        } while (0)

#define NUM_NS 7
#define MAX_PATH_LEN 256

static char *nsNames[] = {
	"mnt",
	"net",
	"pid",
	"uts",
	"ipc",
	"user",
	"cgroup"
};

int main(int argc, char *argv[])
{
  pid_t pid = atoi(argv[1]);
  printf("List of namespaces for process %d\n", pid);
  for(int i=0; i<NUM_NS; i++)
  {
    char nsPath[MAX_PATH_LEN];
    sprintf(nsPath, "/proc/%d/ns/%s", pid, nsNames[i]);
    char nsInode[MAX_PATH_LEN];
    ssize_t numRead = readlink(nsPath, nsInode, MAX_PATH_LEN-2);
    if (numRead == -1) errExit("readlink");
    nsInode[numRead] = '\0';
    printf("%s\t\t---->\t\t%s\n", nsNames[i], nsInode);
  }
  exit(EXIT_SUCCESS);
}
