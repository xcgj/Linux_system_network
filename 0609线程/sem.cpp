/*************************************************************************
  > File Name: conditionvar.cpp
  > Author: C01
  > Mail: 1003192057@qq.com 
  > Created Time: 2017年06月10日 星期六 14时15分05秒
 ************************************************************************/
#include"../header.h"
#include<iostream>
#include<list>
using namespace std;

//数据
struct data
{
    int a;
};

list<data*> ls;

//自动加锁解锁类
class LOCK
{
    private:
        pthread_mutex_t & m;
    public:
        LOCK(pthread_mutex_t & mutex):m(mutex)
    {
        pthread_mutex_lock(&m);
    }

        ~LOCK()
        {
            pthread_mutex_unlock(&m);
        }
};

//锁
pthread_mutex_t mutex;

//信号量定义
sem_t sem;

void* senddata(void * ptr)
{
    //发信号，让sem的值加1
    sem_post(&sem);

    int i = 0;
    while(1)
    {
        usleep(100*1000);
        data * t = new data;
        t->a = i++;
        {
            LOCK lock(mutex);
            ls.push_back(t);
        }
    }
}

void* gaindata(void * ptr)
{
    //等待信号量，如果sem的值是0，那么阻塞
    sem_wait(&sem);

    data * t;
    while(1)
    {
        usleep(50*1000);
        {
            LOCK lock(mutex);
            if (ls.size() == 0)continue;
            else
            {
                t = ls.front();
                ls.pop_front();
            }
        }
        printf("a = %d\n", t->a);
        delete t;
    }
}

int main()
{
    //初始化信号量
    sem_init(&sem, 0, 0);

    //锁
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex, &attr);

    //线程
    pthread_t tid1;
    pthread_t tid2;
    pthread_create(&tid1, NULL, senddata, NULL);
    pthread_create(&tid2, NULL, gaindata, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}
