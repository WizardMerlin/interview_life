#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

/*static init*/
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t has_product = PTHREAD_COND_INITIALIZER;

struct msg{
  int num;
  struct msg *next;
};

struct msg *head = NULL;
struct msg *tmp = NULL;

void *productor(void *arg)
{

  for(;;) {
    tmp = (struct msg*) malloc(sizeof(struct msg));

    tmp->num = rand() % 100 + 1;
    printf("---producer has produced %d \n", tmp->num);

    pthread_mutex_lock(&mutex);
    tmp->next = head;
    head = tmp;
    pthread_cond_signal(&has_product); /*producer Vs consumer = 1:1*/
    pthread_mutex_unlock(&mutex);

    usleep(rand()%1000); //给消费者机会
  }
  
  return NULL;
}

void *consumer(void *arg)
{

  for(;;) {

    /*开始操作Msg队列*/
    pthread_mutex_lock(&mutex);

    while(head==NULL) { //队列为空, 放弃锁, 阻塞, 等待下一波唤醒
      pthread_cond_wait(&has_product, &mutex);
    }

    //有产品, 取走一个队头
    tmp = head;
    head = head->next;
    pthread_mutex_unlock(&mutex);

    printf("consumer  has consumed %d \n", tmp->num);
    free(tmp);
    usleep(rand()%1000); //给生产者机会    
  }
  
  return NULL;  
}




int main(void) 
{

  pthread_t ptid, ctid; //processer and consumer

  /*remember checking return code*/
  pthread_create(&ptid, NULL, productor, NULL);
  pthread_create(&ctid, NULL, consumer, NULL);

  pthread_join(ptid, NULL);
  pthread_join(ctid, NULL);
  
  return 0;
}
