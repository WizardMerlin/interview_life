一个进程最多能创建多少个线程？
1. 首先ulimit -s 看下一个线程的栈空间是多大---默认是8M
2. 再看一下系统或者你设置的栈的大小是多少（或者运行时你自己去指定也可以，或者查看代码，默认2M）

或者写程序自己跑一下，代码思想如下： 主线程不断的创建子线程，计数器累加；创建出来的子线程不能结束.

```c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void *my_func(void *arg){
    while(1){
        sleep(1);
    }
}

int main(void)
{

    pthread_t t;
    int ret, count =1;
    pthread_attr_t attr;

    size_t current_stack_size; //in bytes
    double per_stack_size;

    pthread_attr_init(&attr);

#if 0
    pthread_attr_getstacksize(&attr, &current_stack_size);
    per_stack_size = (double)current_stack_size/(1024.0*1024); //in M, default 8M
    printf("current stack size = %f M\n", per_stack_size); //B-->kB-->M
#endif

    for(;;){
        ret = pthread_create(&t, &attr, my_func, NULL);
        if(ret){
            fprintf(stderr, "create error %s\n", strerror(ret));
            break;
        }

        ++count;
    }

    //printf("should almost be : %d\n", (int)(8/per_stack_size));
    printf("total thread : %d\n", count);
    pthread_attr_destroy(&attr);

    return 0;
}

```

这里还是可以扩展的，这个值是怎么算出来的？
这个值难道不能扩大了么？

设置stack_addr会怎么样？

```c++
int main(){

//..
size_t stacksize;
void *stackaddr;
pthread_attr_t attr;
pthread_attr_init(&attr)  ;


while(1){

	//不停的从堆上分配
	stackaddr = malloc(SIZE); //注意强转
	if(stackaddr == NULL){
	    perror("malloc");
	    exit(-1);
	}
			
	stacksize = SIZE;
	pthread_attr_setstack(&attr, stackaddr, stacksize);  //修改线程栈空间的大小
			
	err = pthread_create( &tid, &attr, my_func, NULL);
	if(err != 0){
	    //...
	    break;
	}
				
	printf("%d\n", count);
}

pthread_attr_destroy(&attr);

}

```