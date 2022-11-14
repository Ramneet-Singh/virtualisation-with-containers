#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <limits.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

/* A simple error-handling function: print an error message based
   on the value in 'errno' and terminate the calling process */

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                        } while (0)

#define STACK_SIZE (1024 * 1024)

static int
childFunc(void *arg)
{
    static int first_call = 1;
    long level = (long) arg;

    if (!first_call) {

        char mount_point[PATH_MAX];

        snprintf(mount_point, PATH_MAX, "/proc%c", (char) ('0' + level));

        mkdir(mount_point, 0555);       /* Create directory for mount point */
        if (mount("proc", mount_point, "proc", 0, NULL) == -1)
            errExit("mount");
        printf("Mounting procfs at %s\n", mount_point);

        // fork a busy child
        pid_t child_pid = fork();
        if (child_pid==-1) errExit("fork");
        if (child_pid==0) execlp("sleep", "sleep", "infinity", (char *)NULL);
    }

    first_call = 0;

    if (level > 0) {

        /* Recursively invoke childFunc() to create another child in a
           nested PID namespace */

        level--;
        pid_t child_pid;

        char *stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE,
                           MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
        if (stack == MAP_FAILED)
            errExit("mmap");

        child_pid = clone(childFunc,
                          stack + STACK_SIZE, /* Assume stack grows downward */
                          CLONE_NEWPID | SIGCHLD, (void *) level);

        if (child_pid == -1)
            errExit("clone");

        // wait for 100 seconds before killing
        if (sleep(100)==-1) errExit("sleep");

        if (kill(child_pid, SIGKILL)==-1)
        {
            printf("%d\n", child_pid);
            errExit("kill");
        } else printf("x%d\n", child_pid);
        if (munmap(stack, STACK_SIZE) == -1)
            errExit("munmap");
    } else {

        /* Tail end of recursion: execute sleep(1) */

        printf("Final child sleeping\n");
        execlp("sleep", "sleep", "100", (char *) NULL);
        errExit("execlp");
    }

    return 0;
}

int
main(int argc, char *argv[])
{
    long levels = atoi(argv[1]);
    childFunc((void *) levels);

    return 0;
}
