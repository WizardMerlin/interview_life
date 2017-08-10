#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 5

struct hands
{
  //按照顺序前面N-1个哲学家线程抢left, 最后一个抢right
  int left; 
  int right;
};

pthread_t t[N]; //5个哲学家线程
pthread_mutex_t m[N]; //5把锁
struct hands h[N];//5个线程左右手对应锁的编号


void * func(void *arg)
{
  int pthread_num = (int) arg;
  int left = h[pthread_num].left;
  int right = h[pthread_num].right;

  while (1) {
    pthread_mutex_lock(&m[left]);
    if (pthread_mutex_trylock(&m[right])) {
      printf("thread[%d] lock right failed, relese left lock %d\n", pthread_num, left);
    } else {
      printf("thread[%d] start eating\n", pthread_num);
      pthread_mutex_unlock(&m[right]);
    }
    pthread_mutex_unlock(&m[left]);
    //休息20纳秒再抢(不给别人机会很可能自己一直在loop)
    //给别人机会越多, 大家不容易冲突; 睡的少, 容易不停放锁
    usleep(20); 
  }
  return NULL;
}


int main(void)
{

  
  int i;
  
  //初始化
  for(i=0; i<N; i++) {
    //初始化锁
    if (pthread_mutex_init(&m[i], NULL) ) {
      fprintf(stderr, "mutex[%d] init error\n", i);
      exit(-1);
    }
    
    //初始化哲学家线程应该抢的锁
    h[i].left = i;
    h[i].right = (i+1)%N;
    
    //初始化线程
    if ( pthread_create(&t[i], NULL, func, (void*)i) ) {
      fprintf(stderr, "thread[%d] init error\n", i);
      exit(-1);
    }
    
  }//end of for init


  //join and destroy
  for(i=0; i<N; i++) {
    pthread_join(t[i], NULL);
  }

  for(i=0; i<N; i++) {
    pthread_mutex_destroy(&m[i]);
  }
  
  return 0;
}

