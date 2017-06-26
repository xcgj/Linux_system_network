/*************************************************************************
	> File Name: createpthread.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月09日 星期五 16时46分15秒
 ************************************************************************/

#include"../header.h"

void * func(void * ptr)//这个线程的执行和主函数的执行没有先后之分，是并行的
{
    usleep(10*1000);//usleep,微秒，暂停10毫秒
    printf("sub\n");
}

int main()
{
    pthread_t thread;
    pthread_create(
            &thread,//线程id
            NULL,//线程的属性
            func,//相当于该线程的main函数，该函数返回则该线程结束
            NULL//上面这个函数的参数
            );

    usleep(10*1000);//usleep,微秒，暂停10毫秒
    printf("main\n");

    sleep(1);
    pthread_join(thread, NULL);

    return 0;
}
