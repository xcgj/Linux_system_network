/*************************************************************************
  > File Name: Logins.c
  > Author: C01
  > Mail: 1003192057@qq.com 
  > Created Time: 2017年06月16日 星期五 14时22分17秒
 ************************************************************************/

#include"../network.h"
#include<list>
using namespace std;

//全局链表，文件描述符队列
list<int> socks;
//锁
pthread_mutex_t mutex;
pthread_cond_t cond;

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

//处理数据详细步骤
void recvData(int newfd)
{
    //接受数据
    // 长度r|name|pass

    //读取内容
    char * buf = myReadBuf(newfd);
    //printf("buf = %s\n", buf);
    //分离内容
    char* saveptr;
    const char* type = strtok_r(buf, "|", &saveptr);
    const char* user = strtok_r(NULL, "|", &saveptr);
    const char* pass = strtok_r(NULL, "|", &saveptr);

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
    free(buf);
    close(newfd);

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
    int epollfd = epoll_create(1024);
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
