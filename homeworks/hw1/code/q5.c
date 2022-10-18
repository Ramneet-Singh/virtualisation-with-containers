// child_exec.c: run a child process.
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int main(void){
  char *child_argv[] = {"ls","-l","-ah",NULL};
  char *child_cmd = "ls";
  printf("Running command '%s'\n",child_cmd);
  printf("------------------\n");
  pid_t child = fork(); // fork a child to execute command
  if (child==-1)
  {
    perror("error in fork");
    exit(EXIT_FAILURE);
  }
  if (child==0) execvp(child_cmd,child_argv);
  int child_status;
  pid_t wpid = waitpid(child, &child_status, 0); // blocking wait for child
  if (wpid==-1)
  {
    perror("error in waitpid");
    exit(EXIT_FAILURE);
  }
  printf("------------------\n");
  printf("Child Finished\n");         // show that the child process has finished
  return 0;
}
