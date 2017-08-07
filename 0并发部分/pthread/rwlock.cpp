#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


int counter = 0; //shared resource
pthread_rwlock_t rwlock;


void *thread_write(void *arg)
{
  int t;
  int i = (int)arg;

  while(1) {
    t = counter;
    usleep(1000);

    pthread_rwlock_wrlock(&rwlock); //write lock
    printf("---write %d: %lu: counter=%d ++counter=%d\n", i, pthread_self(), t, ++counter);
    pthread_rwlock_unlock(&rwlock);
    usleep(2000);
  }

  return NULL;
}

void *thread_read(void *arg)
{
  int i = (int)arg;

  while(1) {
    
    pthread_rwlock_rdlock(&rwlock);
    printf("--------------------read %d: %lu: %d\n", i, pthread_self(), counter);
    pthread_rwlock_unlock(&rwlock);
    
    usleep(900);
  }

  return NULL;
}

int main(void) 
{

  int i;
  pthread_t tid[8]; //3 write, 5 read

  pthread_rwlock_init(&rwlock, NULL);

  for(int i = 0; i<3; i++) {
    pthread_create(&tid[i], NULL, thread_write, (void*)i);
  }

  for(int i =0; i<5; i++) {
    pthread_create(&tid[i+3], NULL, thread_read, (void*)i);
  }

  for(i = 0; i<8; i++) {
    pthread_join(tid[i], NULL);
  }

  pthread_rwlock_destroy(&rwlock);
  


  return 0;
}
