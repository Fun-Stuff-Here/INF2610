/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>  // pour printf
#include <stdlib.h>  // pour rand and srand
#include <time.h> // pour time  
#include <unistd.h>  // pour write, read, pipe and close
#include <sys/wait.h> // pour wait
#include <fcntl.h> // pour open
#include <sys/types.h>  // pour mkfifo
#include <sys/stat.h>  // pour mkfifo
int main() {
   mkfifo("tube",0600);
   int fd = open("tube",O_RDONLY);
   if(fork()==0) {
      int fd2 = open("tube",O_WRONLY);
      write(fd2,"Bonjour",7);
      close(fd2);
      _exit(0);
   }
   char buf[100];
    while(read(fd,&buf,7)>0) 
            printf("Fils: m=%s\n",buf);
    close(fd);
    wait(NULL);
   return 0;
}

