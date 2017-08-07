#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex;

void *func(void *arg)
{

  srand(time(NULL));

  while(1) {
    pthread_mutex_lock(&mutex);
    printf("small");
    sleep(rand()%2);

    printf("\n");
    pthread_mutex_unlock(&mutex);
    sleep(rand()%2);
  }

  return NULL;
}


int main(void)
{
  pthread_t tid;
  int ret;

  srand(time(NULL));

  if(pthread_mutex_init(&mutex, NULL)) {//mutex == 1
    fprintf(stderr, "mutex init error");
    exit(-1);
  }

  ret = pthread_create(&tid, NULL, func, NULL/*(void*)i*/);
  if(ret) {
    fprintf(stderr, "pthread_create error:%s\n", strerror(ret));
    exit(-1);
  }


  while(1) {    
    pthread_mutex_lock(&mutex);
    printf("BIG");
    sleep(rand()%2);

    printf("\n");
    pthread_mutex_unlock(&mutex);
    sleep(rand()%2);
    
  }

  pthread_mutex_destroy(&mutex);

  
  return 0;
}
