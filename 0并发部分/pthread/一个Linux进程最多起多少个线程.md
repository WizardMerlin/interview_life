这个问题的关键在于设置线程栈大小, 以及在一个进程中如何循环创建线程.

不多说, 直接上代码

```c++
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *func(void *arg)
{
  //int i = (int)arg;
  //sleep(i);
  //printf("thread: thread_id = %lu\n", pthread_self());
  int i = 0;
  return NULL;
}


int main(void)
{
  pthread_t tid;
  int ret;
  unsigned long i=1;

  for(;;) {
    ret = pthread_create(&tid, NULL, func, NULL/*(void*)i*/);
    if(ret) {
      fprintf(stderr, "pthread_create error:%s\n", strerror(ret));
      printf("now thread sum number = %lu\n", i+1); //include main thread
      exit(-1);
    }
    i++;
  }
  
  return 0;
}

```