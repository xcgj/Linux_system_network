/*************************************************************************
	> File Name: syn.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月09日 星期五 18时54分14秒
 ************************************************************************/

#include"../header.h"

int a = 0;//全局变量
int b = 0;
pthread_mutex_t mutex;//定义锁变量
pthread_mutex_t mutex2;

void * func(void * ptr)
{
    int i;
    for(i = 0; i < 10000; ++i)
    {
        //加锁
        pthread_mutex_lock(&mutex);
        ++a;//既会影响别人，也会被别人影响
        //解锁
        pthread_mutex_unlock(&mutex);

        pthread_mutex_lock(&mutex2);
        b++;
        pthread_mutex_unlock(&mutex2);
    }
}

int main()
{
    pthread_mutex_init(&mutex, NULL);//初始化锁
    pthread_mutex_init(&mutex, NULL);
    pthread_t tid;
    pthread_create(&tid, NULL, func, NULL);

    int i; 
    for(i = 0; i < 10000; ++i)
    {
        //加锁
        pthread_mutex_lock(&mutex);
        ++a;
        //解锁
        pthread_mutex_unlock(&mutex);

        pthread_mutex_lock(&mutex2);
        b++;
        pthread_mutex_unlock(&mutex2);
    }

    //printf("b = %d\n", b);//可能子线程没有计算完就执行这一句了，导致结果不对
    void * ret;
    pthread_join(tid, &ret);

    printf("a = %d\n", a);
    printf("b = %d\n", b);

    return 0;
}
