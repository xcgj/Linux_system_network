#include"../header.h"
#include <list>
using namespace std;
#define PORT 8888
#define ADDR "0.0.0.0"

list<int> socks;
pthread_mutex_t mutex;
pthread_cond_t cond;

//将新的fd以监听的方式加入到epoll_add所指的实例中，epoll事件注册函数
void epoll_add(int epollfd, int sockfd, int events)
{
    struct epoll_event ev;
    ev.data.fd = sockfd;
    ev.events = events;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev);
}

void * thread_func(void * ptr)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        pthread_mutex_unlock(&mutex);

        while(1)
        {
            pthread_mutex_lock(&mutex);
            if(socks.size() == 0)
            {
                pthread_mutex_unlock(&mutex);
                break;
            }
            int newfd = socks.front();
            socks.pop_front();
            pthread_mutex_unlock(&mutex);

            char buf[4096] = {0};
            int ret = read(newfd, buf, sizeof(buf));
            if(ret <= 0)
                break;
            
            char repbuf[4096] = {0};
            if(strcmp(buf, "HELLO") == 0)
                strcpy(repbuf, "world");
            else if(strcmp(buf, "WORLD") == 0)
                strcpy(repbuf, "hello");
            else
                strcpy(repbuf, buf);

            write(newfd, repbuf, strlen(repbuf));
        }
    }
}
int main()
{
    //服务器对象
    int server = socket(AF_INET, SOCK_STREAM, 0);
    if(server < 0) return -1;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(ADDR);

    int bindret = bind(server, (struct sockaddr*)&addr, sizeof(addr));
    if(bindret < 0) return -2;

    listen(server, 20);

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    //创建线程
    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);
    
    //epoll创建
    int epollfd = epoll_create(1024);
    epoll_add(epollfd, server, EPOLLIN);//集合注册fd

    struct epoll_event ev[5];
    while(1)
    {
        int ret = epoll_wait(epollfd, ev, 5, 5000);
        if(ret > 0)
        {
            int i;
            for(i=0; i<ret; ++i)
            {
                if(ev[i].data.fd == server)//新客户端连接
                {
                    int newfd = accept(server,NULL, NULL);
                    epoll_add(epollfd, newfd, EPOLLIN|EPOLLONESHOT);
                }
                else
                {
                    pthread_mutex_lock(&mutex);
                    socks.push_back(ev[i].data.fd);
                    pthread_mutex_unlock(&mutex);

                    pthread_cond_signal(&cond);
                }
            }
        }
    }

    close(server);
    return 0;
}
