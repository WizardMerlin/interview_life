#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


pthread_key_t key;  //线程私有数据

void destructor(void *t)
{
  int tmp = (int)t;
  printf("destructor executed out of thread %lu, value:%d\n", pthread_self(), tmp);
}


void *func1(void *arg)
{
  int i = 10;
  pthread_setspecific(key, (void*)i);
  printf("thread %lu has set key to %d, address are %x\n", pthread_self(), i, &i);
  sleep(2); //让另外一个线程有机会修改
  return NULL;
}


void *func2(void *arg)
{
  int i = 20;
  sleep(1); //先让func1设置key
  pthread_setspecific(key, (void*)i);
  printf("thread %lu has set key to %d, address are %x\n", pthread_self(), i, &i);

  return NULL;
}

int main(void)
{

  pthread_t tid1, tid2;
  pthread_key_create(&key, destructor);

  pthread_create(&tid1, NULL, func1, NULL);
  pthread_create(&tid2, NULL, func2, NULL);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  pthread_key_delete(key); 
  
  return 0;
}


