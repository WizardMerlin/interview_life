#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>

/*封装mutex*/
struct mt {
  int num;
  pthread_mutex_t mutex;
  pthread_mutexattr_t mutexattr;
};


int main(void)
{
  int i;
  struct mt *mm; //共享匿名内存映射区(父子线程共享锁)

  pid_t pid;

  /*
    //使用文件做共享内存
    int fd = open("mt_test", O_CREATE | O_RDWR, 0777);
    ftruncate(fd, sizeof(*mm));

    mm = mmap(NULL, sizeof(*mm), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    unlink("mt_test");
   */
  mm = mmap(NULL, sizeof(*mm), PROT_READ | PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);
  memset(mm, 0, sizeof(*mm));

  /*把mutex属性设置为进程同步属性*/
  pthread_mutexattr_init(&mm->mutexattr);
  pthread_mutexattr_setpshared(&mm->mutexattr, PTHREAD_PROCESS_SHARED);

  pthread_mutex_init(&mm->mutex, &mm->mutexattr);


  pid = fork();
  if (pid == 0) { //子线程
    for (i = 0; i<10; ++i) {
      pthread_mutex_lock(&mm->mutex);
      mm->num += 1;
      printf("---child ---- num++ %d\n", mm->num);
      pthread_mutex_unlock(&mm->mutex);
      sleep(1); //留机会给福线程
    }
  } else if (pid > 0) {
    for ( i = 0; i < 10; ++i) {
      sleep(3);
      pthread_mutex_lock(&mm->mutex);
      mm->num += 2;
      printf("-----father----num +=2 %d\n", mm->num);
      pthread_mutex_unlock(&mm->mutex);
    }
    wait(NULL);
    
  } else {
    fprintf(stderr, "fork error, exit");
    exit(-1);
  }
  

  //销毁工作
  pthread_mutexattr_destroy(&mm->mutexattr);
  pthread_mutex_destroy(&mm->mutex);
  munmap(mm, sizeof(*mm));
  
  return 0;
}
