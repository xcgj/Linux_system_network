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
//生产者链表容器保存、分配任务， 消费者链表接受任务
//生产者链表发数据给链表
//消费者链表任务执行完毕发学习给生产者
//利用管道实现数据交换



//数据
struct data
{
    int a;
};

//链表
list<data*> * listsend;//用指针方便数据信息交换
list<data*> * listgain;

//管道
int fd[2];//任务信号传输管道
int sig[2];//交换信号通信管道
//pipe(fd);

//生产者
//链表需要交换的2种情况：
// 主线程任务过来了，生产者接受后，检查发现消费者的队列是空的
// 子线程发消息过来请求交换链表，检查发现生产者的队列不为空才交换
// 做这两个交换判断的原因：即使用了continue判空消费者队列，由于程序启动之初两个链表都是空的，会造成生产者和消费者信息传递和队列交换的死循环
void* senddata(void * ptr)
{
    int needswap = 0;//检查是否需要交换
    int i = 0;
    while(1)
    {
        //usleep(100*1000);//100毫秒
        char buf;
        //生产者从管道提取别的线程放入的任务，或者消费者发来的消息
        //如果管道没有数据，会阻塞等待，但不占用cpu
        read(fd[0], &buf, 1);//从管道中读取一个字节

        //处理收到的消息
        if (buf == 2)
        {
            //从外部接受任务
            data *d = new data;
            d->a = i++;
            listsend->push_back(d);

            //检查消费者队列是否为空，是则交换
            //if (listgain->size() == 0)
            if (needswap)//只要针对程序启动时使用，由于生产者队列也是空的，如果之前记录了消费者需要交换的请求，等生产者队列有数据了就直接交换
            {
                //交换生产者和消费者的队列
                list<data*> * tmp = listsend;
                listsend = listgain;
                listgain = tmp;
                //交换完成，给sig管道发个信号通知消费者消费者队列有数据了，可以处理了
                write(sig[1], &buf, 1);

                needswap = 0;//数据交换完成，清除请求，防止重复交换

            }
        }

        if (buf == 1)
        {
            //消费者请求交换队列，检查生产者队列是否有任务
            if (listsend->size() > 0)
            {
                //交换生产者和消费者的队列
                list<data*> * tmp = listsend;
                listsend = listgain;
                listgain = tmp;
                //交换完成，给sig管道发个信号通知消费者消费者队列有数据了，可以处理了
                write(sig[1], &buf, 1);

                needswap = 0;//数据交换完成，清除请求，防止重复交换

                /*data *tmp = listsend->front();//从生产者队列提取任务
                  listsend->pop_front();//生产者队列删除任务
                  listgain->push_back(tmp);//任务递达消费者队列*/
            }
            else//生产者队列没数据，先记录一下交换请求，有数据了直接交换
            {
                needswap = 1;
            }
        }
    }
}

//消费者
void* gaindata(void * ptr)
{
    data *tmp;
    while(1)
    {
        if(listgain->size() == 0)
        {
            //发送交换消息，从生产者链表获取数据
            char buf = 1;
            write(fd[1], &buf, 1);

            //监听管道，如果对方链表交换完成，会发送数据过来
            //如果没数据，会阻塞等待
            read(sig[0], &buf, 1);
            //continue;//不加continue会发生段错误,continue用来检查链表是否为空。特别是在刚开始程序，链表若没做交换条件判断，两个链表都为空的情况下，会段错误

            /*不好，容易死循环
            //信号发送后，进入交换等待死循环，直到链表交换完成
            while(listgain->size() == 0)
            {
            usleep(50*1000);//50毫秒
            }*/
        }
        tmp = listgain->front();
        //tmp = *listgain->begin();//lis->begin()是指向lis容器第一个元素的迭代器指针
        listgain->pop_front();
        printf("a = %d\n", tmp->a);
        delete tmp;
    }
}

int main()
{
    //链表申请内存空间
    listsend = new list<data*>;
    listgain = new list<data*>;

    //管道初始化
    pipe(fd);
    pipe(sig);

    //线程创建
    pthread_t send;
    pthread_t gain;
    pthread_create(&send, NULL, senddata, NULL);
    pthread_create(&gain, NULL, gaindata, NULL);

    //主线程给生产者发送任务
    while(1)
    {
        getchar();

        char buf = 2;
        //给管道发送消息
        write(fd[1], &buf, 1);
    }

    //线程回收
    void * ret;
    pthread_join(send, &ret);
    pthread_join(gain, &ret);

    return 0;
}
