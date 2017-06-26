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

//条件变量
pthread_cond_t g_cond;

void* senddata(void * ptr)
{
    //signal只能唤醒正在等待信号的wait，因此需要延迟
    sleep(1);

    //唤醒条件变量
    pthread_cond_signal(&g_cond);

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
    //等待条件变量
    //必须和锁配合使用
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&g_cond, &mutex);
    /* 加锁代码
     * pthread_mutex_unlock(&mutex);
     * wait..信号
     * pthread_mutex_lock(&mutex);
     * 加锁代码
     */
    pthread_mutex_unlock(&mutex);

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
    //初始化条件变量
    pthread_cond_init(&g_cond, NULL);

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
