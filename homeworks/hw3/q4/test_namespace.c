/* demo_userns.c

   Copyright 2013, Michael Kerrisk
   Licensed under GNU General Public License v2 or later
*/
#define _GNU_SOURCE
#include <sys/wait.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                        } while (0)

static int                      /* Startup function for cloned child */
childFunc(void *arg)
{
	sleep(5);
	fflush(stdout);
	char *const args[] = {"./simple_init",NULL};
	if(execv(args[0],args)<0){
		errExit("exec simple_init");
	}
    	return 0;
}

#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];    /* Space for child's stack */

int
main(int argc, char *argv[])
{
    pid_t pid;

    /* Create child; child commences execution in childFunc() */
    fflush(stdout);
    pid = clone(childFunc, child_stack + STACK_SIZE,    /* Assume stack
                                                           grows downward */
                CLONE_NEWPID | CLONE_NEWUTS | CLONE_NEWUSER | SIGCHLD, argv[1]);
    if (pid == -1)
        errExit("clone");
	
    printf("%d\n",pid);

    char uid_file[80];
    char setgroups_file[80];
    char gid_file[80];

    sprintf(uid_file, "/proc/%d/uid_map", pid);
    sprintf(setgroups_file, "/proc/%d/setgroups", pid);
    sprintf(gid_file, "/proc/%d/gid_map", pid);

    int fd;
    fd = open(uid_file, O_WRONLY);
    dprintf(fd, "0 1000 1");
    close(fd);

    fd = open(setgroups_file, O_WRONLY);
    dprintf(fd, "deny");
    close(fd);

    fd = open(gid_file, O_WRONLY);
    dprintf(fd, "0 1000 1");
    close(fd);
    /* Parent falls through to here.  Wait for child. */

    if (waitpid(pid, NULL, 0) == -1)
        errExit("waitpid");

    exit(EXIT_SUCCESS);
}
