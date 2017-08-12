#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define SIZE 3  //队列总大小(最多生产SIZE-1个)


//封装产品队列条件结构体
typedef struct cond_struct
{
  pthread_cond_t notfull;
  pthread_cond_t notempty;

  pthread_mutex_t lock; //绑定条件变量的Mutex

  int buffer[SIZE]; //产品队列(数组)
  int readpos; //消费位置
  int writepos; //生产位置
  
}cond_t;


void init(cond_t*); //初始化 cond_t 条件&产品队列结构体

void put(cond_t *, int); //"放入产品"函数的封装(注意检测满的情况)
int get(cond_t *); //"取出产品"函数的封装(注意检测空的情况)


void *produce(void*);
void *consume(void*);

cond_t queue; //全局队列

/*主函数(主线程负责回收)*/
int main(void) 
{
  pthread_t producer, consumer;

  init(&queue);
  
  pthread_create(&producer, NULL, produce, NULL);
  pthread_create(&consumer, NULL, consume, NULL);

  pthread_join(producer, NULL);
  pthread_join(consumer, NULL);
  
  return 0;
}


void init(cond_t *arg)
{
  pthread_mutex_init(&(arg->lock), NULL);

  pthread_cond_init(&(arg->notempty), NULL);
  pthread_cond_init(&(arg->notfull), NULL);

  arg->readpos = 0;
  arg->writepos = 0;
}


/*放入和取出注意一下是存储结构是数组, 而不是链表*/
void put(cond_t *queue, int data)
{
  pthread_mutex_lock(&(queue->lock));

  //生产之前判断不为满
  while( (queue->writepos+1)%SIZE == queue->readpos ) {
    //说明队列满了
    printf("producer watting for the queue not full\n");
    pthread_cond_wait(&(queue->notfull), &(queue->lock));
  }

  //队列没有满的情况, 直接生产
  queue->buffer[queue->writepos] = data;
  queue->writepos = (queue->writepos + 1) % SIZE;
  printf("producer produce data = %d\n", data);
  
  pthread_cond_signal(&(queue->notempty));
  pthread_mutex_unlock(&(queue->lock));    
}

//出队一个产品
int get(cond_t *queue)
{
  int data;
  pthread_mutex_lock(&(queue->lock));

  while( queue->readpos == queue->writepos ) {
    printf("consumer waitting for queue not empty.\n");
    pthread_cond_wait(&(queue->notempty), &(queue->lock));
  }

  //不为空, 直接出队
  data = queue->buffer[queue->readpos];
  queue->readpos = (queue->readpos + 1) % SIZE;
  printf("---consumer consume data = %d\n", data);
  
  pthread_cond_signal(&(queue->notfull));
  pthread_mutex_unlock(&(queue->lock));
  
  return data;
}


void *produce(void *arg)
{
  srand(time(NULL));
  /*可以控制生产速度, 用循环控制生产产品个数*/
  //for(int i = 0; i < n; ++)
  while(1) {
    put(&queue, rand()%10);
    sleep(1);  //1秒生产一个(让生产者稍快)
  }
  
  return NULL;
}

void *consume(void *arg) 
{

  while(1) {
    get(&queue);
    sleep(2);
  }

  return NULL;
}
