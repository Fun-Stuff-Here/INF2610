#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdio.h>

long* pa;
void increment()
{
    for (int i = 0; i < 1000000; i++)
        *pa = *pa + 1;
}

int main()
{
    pa = mmap(NULL, sizeof(long), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    *pa = 0;
    if(fork()==0)
    {
        increment();
        munmap(pa, sizeof(long));
        _exit(0);
    }

    wait(NULL);
    printf("*pa = %ld\n", *pa);
    munmap(pa, sizeof(long));
    return 0;
}