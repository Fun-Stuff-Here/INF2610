/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// fonction de nettoyage 
void mfree (void *arg)  {  printf("free\n"); free(arg); }

// fonction mthread
void *mthread(void *taille)
{     pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);
    // allouer dynamiquement un tableau 
    long*tab = (long*) malloc ( (long)taille*sizeof(long));
    if (tab!=NULL)
    {   pthread_cleanup_push(mfree, (void*) tab);
        // remplir le tableau
        for (long i=0; i<(long)taille; i++) 
            tab[i]=i;
        printf("\n fin de remplissage \n");
         pthread_cleanup_pop(0); 
    }
    // retourner l'adresse du tableau (récupérable via pthread_join) 
    pthread_exit ((void*) tab);
}



int main() {
    pthread_t th;
    long* pstatus;
    if(pthread_create(&th,NULL,mthread,(void*)5))   return 1;
    pthread_join(th, (void**) &pstatus);
    if(pstatus != PTHREAD_CANCELED && pstatus!=NULL ) { 
        printf("Terminaison normale avec pstatus=%p.\n", pstatus);
        // afficher le contenu du tableau
        for(int i=0; i<5;i++)
            printf("tab[%d]=%ld ", i, pstatus[i]);
        printf("\n fin de l'affichage \n");
        // libérer l'espace alloué au tableau
        free(pstatus);
    }
    return 0;
}








