/*************************************************************************
	> File Name: relock.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月09日 星期五 20时19分27秒
 ************************************************************************/

#include"../header.h"

pthread_mutex_t mutex;

//被调函数有锁，造成重复加锁、
void foo()
{
    pthread_mutex_lock(&mutex);
    printf("thread = %d\n", (int)pthread_self());
    pthread_mutex_unlock(&mutex);
}

void * func(void* ptr)
{
    //加锁
    pthread_mutex_lock(&mutex);
    //调函数
    foo();
    //解锁
    pthread_mutex_unlock(&mutex);
}

int main()
{
    //锁初始化
    pthread_mutex_init(&mutex, NULL);

    //创建子线程
    pthread_t tid;
    pthread_create(&tid, NULL, func, NULL);

    //线程回收
    void* ret;
    pthread_join(tid, &ret);

    return 0;
}
