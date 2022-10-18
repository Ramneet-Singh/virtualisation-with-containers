 #include<stdio.h>
 #include <unistd.h>
 #include<stdlib.h>
 
 int i,j;
 int getvalue (int k) {
     return k+1;
 }
 
 int main(int argc, char *argv[]) {
	 if (atoi(argv[1]) == 1) {
	      for (j = 0; j <= 1000000; j++)
		      i = getvalue(j);
	 }
	 else {
	     for (j = 0; j <= 1000000; j++)
	      	printf("%d", getppid());
	 }
 }
