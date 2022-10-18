#include <stdio.h>                                                                                                                            
#include <stdlib.h>                                                                                                                           
#include <sys/types.h>                                                                                                                        
#include <sys/stat.h>                                                                                                                         
#include <fcntl.h>                                                                                                                            
#include <unistd.h>                                                                                                                           
                                                                                                                                              
int main(int argc, char *argv[]) {                                                                                                            
  int fd1 = open("hw1f1", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);                                                                     
  int fd2 = dup(fd1);                                                                                                                         
  int fd3 = open("hw1f1", O_RDWR);                                                                                                            
  int fd4 = open("hw1f1", O_WRONLY | O_APPEND);                                                                                               
  write(fd1, "Hello,", 6);                                                                                                                    
  write(fd2, " world", 6);                                                                                                                    
  lseek(fd2, 0, SEEK_SET);                                                                                                                    
  write(fd4, " Hej da!", 8);                                                                                                                  
  write(fd1, "HELLO,", 6);                                                                                                                    
  write(fd3, "Gidday", 6);                                                                                                                    
  lseek(fd4, 0, SEEK_SET);                                                                                                                    
  write(fd4, " Var ar vi?", 11);                                                                                                              
} 