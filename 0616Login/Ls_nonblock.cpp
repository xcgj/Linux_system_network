/*************************************************************************
  > File Name: Logins.c
  > Author: C01
  > Mail: 1003192057@qq.com 
  > Created Time: 2017年06月16日 星期五 14时22分17秒
 ************************************************************************/

#include"../network.h"
#include<list>
#include<map>
using namespace std;
//程序问题：第二次登录时服务器socket的结构体的buf后面有垃圾值，为什么？？？

//全局链表，文件描述符队列
list<int> socks;
//锁
pthread_mutex_t mutex;
pthread_cond_t cond;

int epollfd;

//将新的fd以监听x的方式加入到epoll_add所指的实例中，epoll事件注册函数
void epoll_add(int epollfd, int sockfd, int events)
{

    //在这里先注册要监听的事件类型。
    //第一个参数是epoll_create()的返回值，
    //第二个参数表示动作，用三个宏来表示：
    //    EPOLL_CTL_ADD：注册新的fd到epfd中；
    //    EPOLL_CTL_MOD：修改已经注册的fd的监听事件；
    //    EPOLL_CTL_DEL：从epfd中删除一个fd；
    //第三个参数是需要监听的fd，
    //第四个参数是告诉内核需要监听什么事
    struct epoll_event ev;
    ev.data.fd = sockfd;
    ev.events = events;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev);
}

//保存文件描述符接收到的可能不完整的数据
struct Channel
{
    int fd;
    char * buf;     //数据包的大小(packetSize)+数据 == 读完的readSize
    int readSize;   //已经读到的数据，如果读完了，大小应该是全部传输数据的长度：packetSize + 4
    int packetSize; //数据包的大小，数据包的字节数，占4个字节大小

    Channel(int fd)
    {
        this->fd = fd;
        buf = new char[4096];
        readSize = 0;
        packetSize = 0;
    }

    ~Channel()
    {
        delete[] buf;
        close(fd);
    }
};

//通过文件描述符找数据
map<int, Channel*> channelmap;

//数据处理
void handleData(Channel * ch)
{
    int newfd = ch->fd;
    char * buf = ch->buf + 4;//将数据头的大小标识偏移
    //buf[ch->packetSize] = 0;

    //接受数据
    // 长度r|name|pass

    //读取内容
    //char * buf = myReadBuf(newfd);
    //printf("buf = %s\n", buf);
    //分离内容
    char* saveptr;
    printf("%d, %d, %s\n", ch->packetSize,ch->readSize, buf);
    const char* type = strtok_r(buf, "|", &saveptr);
    const char* user = strtok_r(NULL, "|", &saveptr);
    const char* pass = strtok_r(NULL, "|", &saveptr);
    printf("%s, %s, %s\n----------------\n", type, user, pass);

    //处理内容
    if(type[0] == 'r')
    {
        //注册, 写入用户名和密码各占一行
        FILE * filefd = fopen("user.data", "a+");
        fprintf(filefd, "%s\n%s\n", user, pass);
        fclose(filefd);
        //回应客户端注册结果 长度r|ok
        myWriteBuf(newfd, "r|ok");            
    }
    else if(type[0] == 'l')
    {
        //登录，遍历文件对比输入
        FILE * fp = fopen("user.data", "r");
        char ubuf[1024];
        char pbuf[1024];
        char * p;
        while(1)
        {
            p = fgets(ubuf, sizeof(ubuf), fp);
            if (p == NULL) break;
            fgets(pbuf, sizeof(pbuf), fp);

            ubuf[strlen(ubuf) - 1] = 0;//去掉\n
            pbuf[strlen(pbuf) - 1] = 0;

            if(strcmp(ubuf, user) == 0 && strcmp(pbuf, pass) == 0)
            {
                myWriteBuf(newfd, "l|ok");
                break;// p!=NULL
            }
            printf("%s, %s\n", ubuf, pbuf);
        }

        if(p == NULL)//遍历整个文件，都没有匹配项
        {
            myWriteBuf(newfd, "l|err");
        }

        fclose(fp);
    }
    else
    {
        //非法数据
        myWriteBuf(newfd, "unknown cmd");
    }
    delete ch;
}

//读取数据包
void readPacket(Channel * ch)
{
    //while???--不需要while，后面数据没读完的话，fd不删除，结构体也不释放
    //          而是状态设置回可通知状态，当下次进入队列被线程重新读取时，
    //          会从之前的结束的位置开始读取
    int ret = read( ch->fd, 
                    ch->buf + ch->readSize,
                    ch->packetSize - ch->readSize + 4); 
    if(ret > 0)
    {
        ch->readSize += ret;
        if(ch->readSize == ch->packetSize + 4)
        {
            handleData(ch);
        }
    }  
    else if(ret < 0 && errno != EAGAIN || ret == 0)//出错 对方关闭socket
    {//非阻塞模式下没有读取到数据--EAGAIN
        pthread_mutex_lock(&mutex);
        channelmap.erase(channelmap.find(ch->fd));//删除元素
        pthread_mutex_unlock(&mutex);

        delete ch;//删除结构体
    }
    else//非阻塞模式下没有读到数据情况 ret == -1 && errno == EAGAIN
    {
        //继续读，可能读不够指定字节数，将fd扔回到集合里面去
        //这里不先关闭newfd不会造成epollfd的集合重复吗？？？
        struct epoll_event ev;
        ev.data.fd = ch->fd;
        ev.events = EPOLLIN|EPOLLONESHOT;//恢复通知属性
        epoll_ctl(epollfd, EPOLL_CTL_MOD, ch->fd, &ev);
    }
}

//处理数据详细步骤
void recvData(int newfd)
{
    //char buf[1024];
    //int ret = read(newfd, buf, sizeof(buf));
    pthread_mutex_lock(&mutex);
    //获得客户端socket传输的数据
    Channel * ch = channelmap[newfd];
    pthread_mutex_unlock(&mutex);

    //处理数据
    //while???--不需要while，后面数据没读完的话，fd不删除，结构体也不释放
    //          而是状态设置回可通知状态，当下次进入队列被线程重新读取时，
    //          会从之前的结束的位置开始读取
    if(ch->readSize < 4)//先读取数据包有多大
    {
        int ret = read( ch->fd, 
                        ch->buf + ch->readSize, 
                        4 - ch->readSize);
        if(ret > 0)//读不够继续读，读满4字节
        {
            ch->readSize += ret;
            if(ch->readSize == 4)//读满4字节了
            {
                uint32_t * p = (uint32_t *)ch->buf;//步长改成4字节
                ch->packetSize = ntohl(*p);//获取数据包的大小，网络序转主机序
                readPacket(ch);
            }
        }
        else if(ret < 0 && errno != EAGAIN || ret == 0)//出错 对方关闭socket
        {//非阻塞模式下没有读取到数据--EAGAIN
            pthread_mutex_lock(&mutex);
            channelmap.erase(channelmap.find(newfd));//删除元素
            pthread_mutex_unlock(&mutex);

            delete ch;//删除结构体
        }
        else//非阻塞模式下没有读到数据情况 ret == -1 && errno == EAGAIN
        {
            //继续读，可能读不够指定字节数，将fd扔回到集合里面去
            //这里不先关闭newfd不会造成epollfd的集合重复吗？？？--就是要保存这个结构体，下次重新进入的时候会从没读完的位置开始读取
            struct epoll_event ev;
            ev.data.fd = newfd;
            ev.events = EPOLLIN|EPOLLONESHOT;//恢复通知属性
            epoll_ctl(epollfd, EPOLL_CTL_MOD, newfd, &ev);
        }
    }
    else if(ch->readSize > 4)//从之前没读完数据的地方继续读数据
    {
        readPacket(ch);
    }
}

//线程函数取链表中的文件描述符处理数据
void * thread_func(void * ptr)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);//必须要和一把锁配合使用等待条件成立，
        //如果条件不成立，那么程序阻塞在这里，不占用CPU，
        //内部会自动解锁，wait成功后，再自动加锁
        pthread_mutex_unlock(&mutex);
        while(1)//可能在这个线程处理数据的过程中，链表增加了新的fd
        {
            pthread_mutex_lock(&mutex);
            if(socks.size() == 0)//将链表中存储的fd处理完，直到没有fd
            {
                pthread_mutex_unlock(&mutex);
                break;
            }
            int newfd = socks.front();  //读fd
            socks.pop_front();          //删fd
            pthread_mutex_unlock(&mutex);
            //处理fd的数据，因为后面来的fd都存在链表中，可以慢慢处理
            //让主线程可以安心接收新连接而不被打扰
            recvData(newfd);

            /*
            //用EPOLL_CTL_MOD解除这个描述符的由于EPOLLONESHOT导致的通知屏蔽状态
            //相当于在集合中重新加入了这个newfd
            //本例中，客户端采用的是短连接，客户端的socket连接一次发完数据就close了，因此newfd再加入集合会造成管道破裂EPIPE
            //如果客户端是长连接，那么适合这样处理，恢复newfd的通知状态
            struct epoll_event ev;
            ev.data.fd = newfd;
            ev.events = EPOLLIN|EPOLLONESHOT;
            epoll_ctl(epollfd, EPOLL_CTL_MOD, newfd, &ev);
            */
        }
    }
}

int main()
{
    //服务器对象
    int server = myServer(9988, "0.0.0.0");
    //保存数据的文件
    close(open("user.data", O_CREAT|O_EXCL, 0777));

    //锁
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    //创建线程
    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);

    //epoll创建
    epollfd = epoll_create(1024);
    epoll_add(epollfd, server, EPOLLIN);//集合注册fd

    struct epoll_event ev[8];
    while(1)
    {
        //监听fds
        int ret = epoll_wait(epollfd, ev, 8, 5000);
        if(ret > 0)
        {
            int i;
            for(i=0; i<ret; ++i)
            {
                if(ev[i].data.fd == server)//连接新客户端
                {
                    int newfd = myAccept(server, NULL, NULL);
                    //将新fd只放入集合，不放入链表，因为新fd暂时没有数据
                    epoll_add(epollfd, newfd, EPOLLIN|EPOLLONESHOT);//注册fd
                    //EPOLLONSHOT：这个描述符有数据时只通知一次就不会再通知了
                    //这样可以避免在以后的监听过程中，这个描述符由于子线程没有及时处理完数据，而被重复地加入到链表中

                    //将每一个新建的fd都设置成非阻塞属性
                    int flags = fcntl(newfd, F_GETFL);
                    flags |= O_NONBLOCK;
                    fcntl(newfd, F_SETFL, &flags);   

                    //数据包信息
                    Channel * ch = new Channel(newfd);
                    channelmap[newfd] = ch;
                }
                else
                {
                    //将有数据接收的fd放入链表给另一个线程处理
                    pthread_mutex_lock(&mutex);
                    socks.push_back(ev[i].data.fd);
                    pthread_mutex_unlock(&mutex);
                    //通知另外一个线程不用等待，可以处理数据了
                    pthread_cond_signal(&cond);
                }
            }
        }
    }

    close(server);
    return 0;
}
