#include <stdio.h>  // pour printf
#include <stdlib.h>  // pour rand and srand
#include <time.h> // pour time  
#include <unistd.h>  // pour write, read, pipe and close
#include <sys/wait.h> // pour wait
#include <fcntl.h> // pour open
#include <sys/types.h>  // pour mkfifo
#include <sys/stat.h>  // pour mkfifo

int main () { 
   mkfifo("inf2610",0600);
   if(fork()==0)  { // premier fils
          int fd = open("inf2610",O_WRONLY);
          dup2(fd,1); close(fd);
            write(1, ”message du fils1\n”, 17); 
            _exit(0);
    }
    if(fork()==0)  { // second fils
            int fd = open("inf2610",O_RDONLY);
            dup2(fd,0); close(fd);
            char c;   
            while (read(0,&c,1) >0)
              write(1,&c, 1);
            _exit(0);
    } 
    int fd = open("inf2610",O_WRONLY);
    dup(fd,1); close(fd);
    /*7*/
   write(1, ”message du pere\n”, 16); 
    while(wait(NULL)>0);
    /*9*/
   return 0;
} 