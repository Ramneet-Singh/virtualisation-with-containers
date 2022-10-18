// wait_loop.c: Demonstrates waiting for a child but does so in a
// blocking fashion.
// 
// NOTE: Make sure to compile sleep_print first as in
// > gcc -o sleep_print sleep_print.c


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(void) {
  char *child_argv[] = {"./sleep_print","5","CHILD: Awake and Done",NULL};
  pid_t child_pid = fork();
  if(child_pid == 0){
    execvp(child_argv[0],child_argv);
    perror("errno indicates");
    printf("Couldn't run '%s': is it compiled??\n",child_argv[0]);
    exit(1);
  }

  while(1){
    printf("Type text then press enter to check on child:\n");
    printf(">> ");                    // print prompt
    char buffer[1024];
    char *input = fgets(buffer,1024,stdin);
    input[strlen(input)-1] = '\0';    // Eliminate newline in input placed by fgets()
    printf("Entered text: '%s'\n",input);

    printf("Waiting\n");
    int status;
    pid_t pid = waitpid(child_pid, &status, 0);
    if(pid == child_pid){
      printf("CHILD FINISHED: ");
      printf("\n");
    }
    else{
      printf("Child not finished: wait returned %d\n",pid);
    }
    printf("\n");
  }
  printf("Exiting wait_loop\n");
  return 0;
}
