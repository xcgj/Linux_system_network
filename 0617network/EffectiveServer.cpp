/*************************************************************************
	> File Name: EffectiveServer.cpp
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月17日 星期六 17时47分07秒
 ************************************************************************/
/* 
 * 多个线程accept，将产生的fd加入epoll
 * epoll监听fd，将有消息的fd加入等待执行队列
 * 多个任务执行线程获取任务
 */
//#include<iostream>
#include"../header.h"
using namespace std;
#include<list>

//=====================全局准备工作========================//
int accept_threads = 4; //调用accept的线程数量
int worker_threads = 8; //处理数据的线程数量

class Channel           //连接客户端通道，保存通道的临时数据
{
public:
    char *      buf;
    uint32_t    packetSize;
    uint32_t    readSize;
    int         fd;

    Channel(int fd):packetSize(0),readSize(0),fd(fd)
    {
        buf = new char[4096];
        memset(buf, 0, sizeof(buf));//传过来的数据不是\0结尾字符串，要置零
    }

    ~Channel()
    {
        close(fd);
        delete[] buf;
    }
};

pthread_mutex_t mutex;  //多线程使用的锁，信号量，消息队列
sem_t sem;
list<Channel*> channellist;

int server;             //服务器文件描述符
int epollfd;            //epoll文件描述符

/*自动锁类*/
class myLock
{
public:
    pthread_mutex_t & mx;
    myLock(pthread_mutex_t & m):mx(m)
    {
        pthread_mutex_lock(&mutex);
    }

    ~myLock()
    {
        pthread_mutex_unlock(&mutex);
    }
};
#define MyLock(mutex)  myLock tmplock(mutex)

//=====================函数调用========================//

void epoll_add(int newfd)
{
    //定义通道
    Channel * ch = new Channel(newfd);
    //epoll事件注册
    struct epoll_event ev;
    ev.data.ptr = ch;
    ev.events = EPOLLIN | EPOLLONESHOT;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, newfd, &ev);
    return;
}

void epoll_mod(Channel * ch)
{
    //epoll事件注册
    struct epoll_event ev;
    ev.data.ptr = ch;
    ev.events = EPOLLIN | EPOLLONESHOT;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, ch->fd, &ev);
    return;
}

/*accept*/
void* accept_thread_func(void* ptr)
{
    while(1)
    {
        int newfd = accept(server, NULL, NULL);
        if(newfd > 0)
        {   //newfd需要设置成非阻塞状态，并注册到epoll集合
            int flags = fcntl(newfd, F_GETFL);
            flags |= O_NONBLOCK;
            fcntl(newfd, F_SETFL, &flags);
            epoll_add(newfd);
        }
    }
    return NULL;
}

Channel* readData(Channel * ch)
{
    int ret;
    if(ch->readSize < 4)
    {   //读数据头
        ret = read( ch->fd,
                    ch->buf + ch->readSize,
                    4 - ch->readSize);
        if(ret > 0)
        {
            ch->readSize += ret;
            if(ch->readSize == 4)
            {   //读满4字节，获取数据包的长度信息
                ch->packetSize = ntohl(*(uint32_t*)(ch->buf));
            }
            return readData(ch);   //递归
        }
    }
    else
    {   //读数据内容
        ret = read( ch->fd,
                    ch->buf + ch->readSize,
                    ch->packetSize + 4 - ch->readSize);   
        if(ret > 0)
        {
            ch->readSize += ret;
            if(ch->readSize == ch->packetSize + 4)
                return ch;          //数据读取完成，回去处理数据
            return readData(ch);    //递归
        }  
    }
    
    if(ret == 0 || (ret < 0 && errno == EAGAIN))
    {/*错误处理*/   //对方关闭了socket或者socket有错误
        delete ch;  //我也关闭socket
    }
    else
    {   //重新设置epoll集合通知状态继续读
        epoll_mod(ch);
    }
    return NULL;
}

void handleData(Channel * ch)
{
    ch->readSize = 0;
    ch->buf[ch->readSize] = 0;
    printf("%s\n", ch->buf + 4);
#if 1   //长连接
    epoll_mod(ch);
#else   //短连接
    delete ch
#endif
    return;
}

/*worker*/
void* worker_thread_func(void* ptr)
{
    while(1)
    {   //工作线程都在等待信号状态
        sem_wait(&sem);
        Channel * ch = NULL;
        {   //操作全局链表加锁
            MyLock(mutex);
            if(channellist.size() == 0)//规定收到信号而无可执行任务就销毁线程
                break;
            ch = *channellist.begin();
            channellist.erase(channellist.begin());

            //printf("listsize = %d, ch->fd = %d\n", channellist.size(), (int)(ch->fd));
        }
        if(readData(ch))//开始读取数据，只要函数返回NULL说明数据还没读完，不会进入处理数据的步骤，而是记录了读取位置内容并重新被设置为通知状态，等待机会
        {   //判断数据读取的完整性
            //printf("%s\n", ch->buf);
            handleData(ch);
        }
    }
    return NULL;
}

//程序初始化
int init()
{
    /*server初始化*/
    server = socket(AF_INET, SOCK_STREAM, 0);
    if(server < 0)
    {
        perror("socket");
        return -1;
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9988);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    int bindret = bind(server, (struct sockaddr*)&addr, sizeof(addr));
    if(bindret < 0)
    {
        perror("bind");
        return -2;
    }
    listen(server, 200);

    //server = myServer(9988, "0.0.0.0");

    /*epoll*/
    epollfd = epoll_create(1024);

    int i;
    pthread_t tid;
    for(i=0; i<accept_threads; ++i)
    {/*创建accept线程*/
        pthread_create(&tid, NULL, accept_thread_func, NULL);
    }
    for(i=0; i<worker_threads; ++i)
    {/*创建worker线程*/
        pthread_create(&tid, NULL, worker_thread_func, NULL);
    }

    /*循环锁*/
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);  //初始化
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);//循环锁属性
    pthread_mutex_init(&mutex, &attr);
    sem_init(&sem, 0, 0);

    return 0;
}

void exec()
{
    struct epoll_event evs[8];
    while(1)
    {
        int ret = epoll_wait(epollfd, evs, 8, 5000);
        //printf("%d\n", ret);
        if(ret > 0)
        {   //有数据传输
            int i;
            for(i=0; i<ret; ++i)
            {   //将有消息的结构体放到队列里面去
                Channel * ch = (Channel *)evs[i].data.ptr;
                {   //加锁，入链表
                    MyLock(mutex);
                    channellist.push_back(ch);
                }  
                sem_post(&sem);//让工作线程抢Channel对象
            }
        }
        else if(ret < 0 && errno != EINTR)
        {   //出错判断
            break;
        }
    }//信号打断和没读到fd都会自动回到起始
}

int main()
{
    init(); //初始化数据
    printf("初始化完成\n");
    exec(); //处理数据
    return 0;
}

