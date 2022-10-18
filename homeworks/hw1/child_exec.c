// child_exec.c: run a child process.
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void){
  char *child_argv[] = {"ls","-l","-ah",NULL};
  char *child_cmd = "ls";
  printf("Running command '%s'\n",child_cmd);
  printf("------------------\n");
  execvp(child_cmd,child_argv);
  printf("------------------\n");
  printf("Child Finished\n");         // show that the child process has finished
  return 0;
}
