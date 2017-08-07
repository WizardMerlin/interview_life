#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>


pthread_mutex_t mutex_a;
pthread_mutex_t mutex_b;



void *func(void*)
{
  pthread_mutex_lock(&mutex_a);
  printf("sub thread, i have a lock, quest for b lock; once i get b lock, i will unlock a\n");  

  pthread_mutex_lock(&mutex_b);
  pthread_mutex_unlock(&mutex_a);
  pthread_mutex_unlock(&mutex_b);
  
  return NULL;
}


int main(void) 
{
  pthread_t tid;
  //int ret;

  /*check return code plz*/
  pthread_mutex_init(&mutex_a, NULL);
  pthread_mutex_init(&mutex_b, NULL);

  pthread_create(&tid, NULL, func, NULL);

  
  pthread_mutex_lock(&mutex_b);
  printf("main thread, i have b lock, quest for a lock; once i get a lock, i will unlock b\n");  
  sleep(1);// let sub thread lock a
  
  pthread_mutex_lock(&mutex_a);
  pthread_mutex_unlock(&mutex_b);
  pthread_mutex_unlock(&mutex_a);

  pthread_mutex_destroy(&mutex_a);
  pthread_mutex_destroy(&mutex_b);
  
}
  
