// fork_list.c: fork a child which lists its pid.
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
  int i;
  for(i = 0; i<8; i++){
    pid_t child = fork();
    if(child == 0){
      break;
    }
  }
  printf("My process number is %d, my pid is %d\n",i,getpid());
  return 0;
}
