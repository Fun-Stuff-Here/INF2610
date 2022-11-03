/*
 * Guerre des lettres
 *
 * Ecole polytechnique de Montreal, GIGL, Automne  2022
 * Julie Labbe 1904635, Nicolas Depelteau 2083544
*/
#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define false 0
#define true 1

int* buffer;
int taille;
int ip = 0;
int ic = 0;

sem_t mutex; // Sémaphore pour la section critique
sem_t occupied; // Sémaphore pour le nombre de cases occupées
sem_t freeSpace; // Sémaphore pour le nombre de cases libres

volatile int flag_de_fin = false;
long sumProd = 0;
long sumCons = 0;

void alarmHandler(int sig) {
   flag_de_fin = true;
}

void* producteur( void* pid) {

   long numberOfNotNull = 0;
   srand(time(NULL));

   do
   {
      sem_wait(&freeSpace); // On attend qu'une case soit libre
      sem_wait(&mutex); // On attend pour la section critique
      int number = rand()%9 + 1;
      buffer[ip] = number;
      if(number != 0) numberOfNotNull++;
      sumProd += number;
      ip = (ip + 1) % taille;
      sem_post(&mutex); // On sort de la section critique
      sem_post(&occupied); // On indique qu'une case est occupée
   } while (!flag_de_fin);

   pthread_exit ((void*) numberOfNotNull);
}

void* consommateur(void *cid) {

   long numberOfNotNull = 0;
   int number = 0;

   do
   {
      sem_wait(&occupied); // On attend qu'une case soit occupée
      sem_wait(&mutex); // On attend pour la section critique
      number = buffer[ic];
      sumCons += number;
      if(number != 0) numberOfNotNull++;
      ic = (ic + 1) % taille;
      sem_post(&mutex); // On sort de la section critique
      sem_post(&freeSpace); // On indique qu'une case est libre
   } while(!flag_de_fin || number != 0);

   pthread_exit ((void*) numberOfNotNull);
}

int main(int argc, char* argv[]) {

   // Le premier argument est le nom du programme
   if (argc != 4) { // si le nombre de paramètres est incorrect 
      printf("Les paramètres du programme sont, dans l’ordre :"
   "le nombre de producteurs, le nombre de consommateurs"
   "et la taille du tampon.\n");
      exit(1);
   }

   int nbProd = atoi(argv[1]);
   int nbCons = atoi(argv[2]);
   taille = atoi(argv[3]);
   sem_init(&occupied, 0, 0);
   sem_init(&mutex, 0, 1);
   sem_init(&freeSpace, 0, taille);
   buffer = malloc(taille*sizeof(int));
   pthread_t* tidProd = malloc(nbProd*sizeof(pthread_t));
   pthread_t* tidCons = malloc(nbCons*sizeof(pthread_t));

   // Création des producteurs et des consommateurs
   for(unsigned long i=0; i<nbProd; i++) pthread_create(&tidProd[i], NULL, producteur, (void*) i);
   for(unsigned long i=0; i<nbCons; i++) pthread_create(&tidCons[i], NULL, consommateur, (void*) i);

   // Alarme
   signal(SIGALRM, alarmHandler);
   alarm(1);

   // Attente de la fin des threads
   long statusProd;
   long sumProdMemorized = 0;
   for(int i=0; i<nbProd; i++){
      pthread_join(tidProd[i], (void**)&statusProd);
      sumProdMemorized += statusProd;
   }
   // On insere le nombre de 0 dans le buffer pour que les consommateurs puissent s'arreter
   for(int i=0; i<nbCons; i++){
      sem_wait(&freeSpace);
      sem_wait(&mutex);
      buffer[ip] = 0;
      ip = (ip + 1) % taille;
      sem_post(&mutex);
      sem_post(&occupied);
   }

   long statusCons;
   long sumConsMemorized = 0;
   for(int i=0; i<nbCons; i++){
      pthread_join(tidCons[i], (void**)&statusCons);
      sumConsMemorized += statusCons;
   }

   //Il affiche ensuite les sommes mémorisées des chiffres produits et consommés.
   printf("Somme des chiffres non nuls produits: %ld \n", sumProdMemorized);
   printf("Somme des chiffres non nuls consommés : %ld \n", sumConsMemorized);

   // Il affiche également les nombres de chiffres produits par les producteurs et consommés par les consommateurs
   printf("Somme des chiffres produits : %ld \n", sumProd);
   printf("Somme des chiffres consommés : %ld \n", sumCons);

   // Libération de la mémoire
   free(tidProd);
   free(tidCons);
   free(buffer);

   // Destruction des sémaphores
   sem_destroy(&mutex);
   sem_destroy(&occupied);
   sem_destroy(&freeSpace);
   return 0;
}
