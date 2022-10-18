// fork_list.c: fork a child which lists its pid.
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>

int main(void) {
  int i;
  for(i = 0; i<8; i++){
    pid_t child = fork();
    if (child==-1)
    {
      perror("Error in fork");
      exit(EXIT_FAILURE);
    }
    if(child == 0){
      break;
    }
    int child_status;
    pid_t wpid = waitpid(child, &child_status, 0); // Blocking wait for child
    if (wpid==-1)
    {
      perror("Error in wait");
      exit(EXIT_FAILURE);
    }
  }
  printf("My process number is %d, my pid is %d\n",i,getpid());
  return 0;
}
