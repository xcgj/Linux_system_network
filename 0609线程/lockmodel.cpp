/*************************************************************************
  > File Name: nolockmodel.cpp
  > Author: C01
  > Mail: 1003192057@qq.com 
  > Created Time: 2017年06月09日 星期五 21时42分58秒
 ************************************************************************/

#include<iostream>
#include"../header.h"
#include<list>
using namespace std;
//两个线程分别作为生产者和消费者
//链表容器保存、给予任务
//生产者发数据给链表
//消费者从链表读数据
//给链表加锁，循环锁，锁自动析构（类）


//锁
pthread_mutex_t mutex;

//数据
struct data
{
    int a;
};

//链表
list<data*> lis;

//自动解锁的类
class LOCK
{
    private:
        pthread_mutex_t &m;
    public:
        LOCK(pthread_mutex_t &mutex):m(mutex)
    {
        pthread_mutex_lock(&m);
    }

        ~LOCK()
        {
            pthread_mutex_unlock(&m);
        }
};

//发送数据
void* senddata(void * ptr)
{
    int i = 0;
    while(1)
    {
        usleep(100*1000);//100毫秒
        data *d = new data;
        d->a = i++;

        {//加锁
            LOCK lock(mutex);
            lis.push_back(d);
        }
    }
}

//读取数据
void* gaindata(void * ptr)
{
    data *tmp;
    while(1)
    {
        usleep(50*1000);

        {
            LOCK lock(mutex);
            if(lis.size() == 0)
            {
                continue;
            }
            else
            {
                tmp = lis.front();
                //tmp = *lis.begin();//lis.begin()是指向lis容器第一个元素的迭代器指针
                lis.pop_front();
            }
        }
        printf("a = %d\n", tmp->a);
        delete tmp;
    }
}

int main()
{
    //初始化锁
    pthread_mutexattr_t attr;//定义锁属性
    pthread_mutexattr_init(&attr);//初始化锁属性
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);//增加锁属性
    pthread_mutex_init(&mutex, &attr);//循环递归锁

    //线程创建
    pthread_t send;
    pthread_t gain;
    pthread_create(&send, NULL, senddata, NULL);
    pthread_create(&gain, NULL, gaindata, NULL);

    //线程回收
    void * ret;
    pthread_join(send, &ret);
    pthread_join(gain, &ret);

    return 0;
}
