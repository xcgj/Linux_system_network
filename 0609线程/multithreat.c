/*************************************************************************
	> File Name: multithreat.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月09日 星期五 17时07分43秒
 ************************************************************************/

#include"../header.h"

void* foo()
{
    pthread_t ptid;
    printf("pthreadid = %d\n", (int)ptid);
}

void* func(void * ptr)
{
    foo();
}

int main()
{
    //线程id
    pthread_t tid1;
    pthread_t tid2;

    //创建线程
    pthread_create(&tid1, NULL, func, NULL);
    pthread_create(&tid2, NULL, func, NULL);

    //线程id比较
    if(pthread_equal(tid1, tid2) == 0)
    {
        printf("线程id不同\n");
    }

    getchar();
    foo();
    sleep(1);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}
