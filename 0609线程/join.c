/*************************************************************************
	> File Name: join.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月09日 星期五 17时33分49秒
 ************************************************************************/

#include"../header.h"

void* func(void* ptr)
{
    sleep(1);
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, func, NULL);;

    void * ret;
    //join会阻塞等待，直到tid这个线程退出
    //退出后回收资源，得到ret返回值
    pthread_join(tid, &ret);

    return 0;
}
