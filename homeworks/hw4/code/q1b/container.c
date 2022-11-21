#define _GNU_SOURCE
#include <sys/wait.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include "ename.c.inc"
#include "error_functions.h"

#define pivot_root(new_root,put_old) syscall(SYS_pivot_root,new_root,put_old)

static int                      /* Startup function for cloned child */
childFunc(void *arg)
{
	sleep(5);
	fflush(stdout);

  printf("Initialising Container...\n");
  char **argv = (char **)arg;
  char *newroot = argv[2];

  mount(NULL, "/", NULL, MS_PRIVATE | MS_REC, NULL);
  printf("Changed mount propagation types\n");

  // mount proc filesystem
  char mountPath[256];
  sprintf(mountPath, "%s/proc", newroot);
  struct stat st = {0};
  if (stat(mountPath, &st) == -1) {
      mkdir(mountPath, 0755);
  }
  if (mount("proc", mountPath, "proc", 0, "") == -1) errExit("mount-proc");
  printf("Mounted proc pseudo-filesystem\n");

  // Pivot root
  char putold[256];
  sprintf(putold, "%s/.putold", newroot);
  struct stat st2 = {0};
  if (stat(putold, &st2) == -1) {
      mkdir(putold, 0700); // Create putold if it doesn't exist
  }
  // Ensure that newroot is a mount point
  if (mount(newroot, newroot, NULL, MS_BIND | MS_REC, NULL) == -1)
    errExit("mount-MS_BIND");
  if (pivot_root(newroot, putold) == -1) errExit("pivot_root");
  // Forget the old root filesystem
  if (umount2("/.putold", MNT_DETACH) == -1) errExit("umount2");
  if (rmdir("/.putold") == -1) errExit("rmdir");

  // Change working directory to newroot
  if (chdir("/") == -1) errExit("chdir");
  // We have now achieved filesystem isolation
  printf("Pivoted to newroot and completely forgotten old root filesystem\n");

  if (mount("", "/", NULL, MS_SHARED | MS_REC, NULL) == -1) errExit("mount-MS_SHARED"); // changes in the container will be propagated to its children if any
  
  // change shell prompt for bash child
  setenv("PS1", argv[3], 1);
  printf("Shell prompt changed\n");
  printf("Forking child to run bash shell\n");

  pid_t childPid = fork();
  if (childPid==-1) errExit("fork");

  if (childPid==0)
  {
    // Run a bash shell
    execlp("bash", "bash", NULL);
    errExit("execlp");
  }

  if (waitpid(childPid, NULL, 0) == -1) errExit("waitpid");
  printf("Forked child finished\n");

  return 0;
}

#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];    /* Space for child's stack */

/*
Arguments 
1. rootForContainerSrc: Source for rootfs filesystem
2. rootForContainerDest: Target where rootfs filesystem will be mounted and which will become new root directory for container
3. promptForContainer: prompt to display in the bash shell inside container
*/
int
main(int argc, char *argv[])
{
    // Bind mount rootfs-src to rootfs-dest
    if (mount(argv[1], argv[2], "", MS_BIND | MS_REC, "")==-1) errExit("mount");
    printf("Bind-mounted %s to %s\n", argv[1], argv[2]);

    pid_t pid;

    /* Create child; child commences execution in childFunc() */
    fflush(stdout);
    pid = clone(childFunc, child_stack + STACK_SIZE,    /* Assume stack
                                                           grows downward */
                CLONE_NEWPID | CLONE_NEWNS | CLONE_NEWUTS | CLONE_NEWUSER | SIGCHLD, argv);
    if (pid == -1)
        errExit("clone");
	
    printf("Cloned Child has PID: %d\n",pid);
    printf("Creating UID & GID Mappings\n");

    char uid_file[80];
    char setgroups_file[80];
    char gid_file[80];

    sprintf(uid_file, "/proc/%d/uid_map", pid);
    sprintf(setgroups_file, "/proc/%d/setgroups", pid);
    sprintf(gid_file, "/proc/%d/gid_map", pid);

    int fd;
    fd = open(uid_file, O_WRONLY);
    dprintf(fd, "0 %d 1", geteuid());
    close(fd);

    fd = open(setgroups_file, O_WRONLY);
    dprintf(fd, "deny");
    close(fd);

    fd = open(gid_file, O_WRONLY);
    dprintf(fd, "0 %d 1", getegid());
    close(fd);
    /* Parent falls through to here.  Wait for child. */
    printf("UID & GID Mappings Created\n");

    if (waitpid(pid, NULL, 0) == -1)
        errExit("waitpid");

    printf("Container finished running\n");
    // Unmount the initial bind mount we had done
    if (umount(argv[2]) == -1) errExit("umount");
    printf("Unmounted the bind-mount of %s to %s we had created initially\n", argv[1], argv[2]);
    exit(EXIT_SUCCESS);
}
