#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*开两个shell同时执行该程序*/
int main(int argc, char *argv[])
{
  int fd;
  struct flock lock;

  /*指定文件名字*/
  if (argc<2) {
    printf("./flock filename\n");
    exit(-1);
  }

  if ((fd = open(argv[1], O_RDWR)) < 0){
    printf("open error");
    exit(-1);
  }
  
  lock.l_type = F_WRLCK; //写锁 (读锁可以同时加锁)
  lock.l_whence = SEEK_SET;
  lock.l_start = 0;
  lock.l_len = 0; //整个文件

  fcntl(fd, F_SETLKW, &lock);
  printf("locked\n");
  
  sleep(5);
  
  lock.l_type |= F_UNLCK;
  fcntl(fd, F_SETLKW, &lock);
  printf("unlocked\n");


  close(fd);
  
  return 0;
}




