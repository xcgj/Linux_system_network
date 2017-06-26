/*************************************************************************
  > File Name: fogetunlock.c
  > Author: C01
  > Mail: 1003192057@qq.com 
  > Created Time: 2017年06月09日 星期五 21时02分02秒
 ************************************************************************/

#include"../header.h"
#include<iostream>
using namespace std;

pthread_mutex_t mutex;

//定义一个类，将加锁解锁放里面
class LOCK
{
    public:
        LOCK(pthread_mutex_t &a):m(a)
    {
        pthread_mutex_lock(&m);
    }

        ~LOCK()
        {
            pthread_mutex_unlock(&m);
        }

    private:
        pthread_mutex_t &m;
};

int g_a = 0;

void * func(void* ptr)
{
    //加锁
    {
        LOCK lock(mutex);
        if (g_a == 1)
        {
            printf("%d\n", g_a);
        }
        else
        {
            ++g_a;
        }
    }

    return NULL;
}

int main()
{   
    //初始化锁
    pthread_mutex_init(&mutex, NULL);

    //创建线程
    pthread_t tid;
    pthread_create(&tid, NULL, func, NULL);

    //加锁
    {
        LOCK lock(mutex);
        ++g_a;   
        printf("main = %d\n", g_a);
    }

    //回收线程
    void* ret;
    pthread_join(tid, &ret);
    return 0;
}
