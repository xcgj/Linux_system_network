/*************************************************************************
  > File Name: mutex.c
  > Author: C01
  > Mail: 1003192057@qq.com 
  > Created Time: 2017年06月08日 星期四 21时13分31秒
 ************************************************************************/

#include"../header.h"
#include<pthread.h>

//定义锁
//如果希望锁在进程间有效果，必须将其定义在共享内存中
pthread_mutex_t* mutex;
void * shm;//共享内存首地址
int isinit = 0;

//创建共享内存
void * createshm()
{
    //打开内存文件
    int fd = shm_open("/mshare", O_RDWR|O_CREAT|O_EXCL, 0777);
    if (fd < 0)
    {
        fd = shm_open("/mshare", O_RDWR, 0777);
    }
    else//创建文件
    {
        ftruncate(fd, 4096);
        isinit = 1;
    }

    void * ptr = mmap(NULL, 4096, PROT_READ|PROT_WRITE,
            MAP_SHARED, fd, 0);
    return ptr;
}


int main()
{
    shm = createshm();
    mutex = (pthread_mutex_t * )shm;//shm前面的内存块存储mutex结构体

    if (isinit)
    {
        //初始化锁
        pthread_mutexattr_t mutexattr;
        pthread_mutexattr_init(&mutexattr);
        pthread_mutexattr_setpshared(&mutexattr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(mutex, &mutexattr);
    }

    //加锁
    pthread_mutex_lock(mutex);

    //不安全操作
    printf("mutex1\n");
    getchar();
    printf("mutex2\n");

    //解锁
    pthread_mutex_unlock(mutex);
    printf("release\n");

    munmap(shm, 4096);
    return 0;

}
