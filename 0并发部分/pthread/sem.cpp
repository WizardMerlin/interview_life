#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


#define NUM 5
int queue[NUM]={0};  //全局队列

sem_t blank_num, product_num; //空余容量, 已生产容量


/*本案例中, 队列用的数组代替, 
 所以队列的移动等操作忽略(可能出现消费者消费产品为0的情况)*/
void *produce(void *arg)
{
  int i = 0;
  while (1) {
    sem_wait(&blank_num); //有空余才生产, 无空余则阻塞
    queue[i] = rand() % 100 + 1; //产生一个产品
    printf("----produced %d\n", queue[i]);   
    sem_post(&product_num);

    i = (i+1) % NUM; //逻辑环
    sleep(rand()%1); //给消费者机会

  }
}

void *consume(void *arg){
  
  int i = 0;
  while (1) {
    sem_wait(&product_num); //有产品才消费, 没有产品则等待
    //队列首部出队
    printf("consume %d\n", queue[i]);
    queue[i] = 0;
    sem_post(&blank_num);

    i = (i+1) % NUM; //逻辑环
    sleep(rand()%3); //给生产者机会多生产
  }
  
}


int main(void)
{
  /*忽略rerurn code的检查*/
  pthread_t pid, cid;
  sem_init(&blank_num, 0, NUM);
  sem_init(&product_num, 0, 0);

  pthread_create(&pid, NULL, produce, NULL);
  pthread_create(&cid, NULL, consume, NULL);

  pthread_join(pid, NULL);
  pthread_join(cid, NULL);

  sem_destroy(&blank_num);
  sem_destroy(&product_num);

  
  return 0;
}
