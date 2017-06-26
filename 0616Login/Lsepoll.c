/*************************************************************************
  > File Name: Logins.c
  > Author: C01
  > Mail: 1003192057@qq.com 
  > Created Time: 2017年06月16日 星期五 14时22分17秒
 ************************************************************************/

#include"../network.h"

//处理连接客户端的soket接收到的数据
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
    close(newfd);//socket关闭，会自动将newfd从epoll中剔除
}

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

int main()
{
    //服务器对象
    int server = myServer(9988, "0.0.0.0");
    //保存数据的文件
    close(open("user.data", O_CREAT|O_EXCL, 0777));

    //创建epoll实例，返回值是文件描述符，这个文件描述符存储的是这个实例的地址
    //epoll实例是怎么实现的？？？
    int epollfd = epoll_create(1024);

    //把server加入epoll实例，事件注册函数
    epoll_add(epollfd, server, EPOLLIN);

    //监听文件描述符
    struct epoll_event ev[8];//收集有动静的文件描述符
    while(1)
    {
        //有动静的描述符的个数
        int ret = epoll_wait(epollfd, ev, sizeof(ev)/sizeof(*ev), 5000);
        if(ret > 0)//wait函数已经将有动静的文件描述符放进ev数组了
        {
            int i;
            for(i=0; i<ret; ++i)
            {
                if(ev[i].data.fd == server)//新的客户端请求连接
                {
                    int newfd = myAccept(server, NULL, NULL);
                    //将newfd也加入注册事件进行监听
                    epoll_add(epollfd, newfd, EPOLLIN);
                }
                else//连接客户端的socket读到数据了
                {
                    recvData(ev[i].data.fd);
                }
            }
        }
        else if(ret == 0)
        {
            //没读到sock数据，时间超时
        }
        else if(ret < 0 && errno == EINTR)
        {
            //信号打断
        }
        else if(ret < 0)
        {
            //严重错误
            printf("error\n");
            break;
        }
    }
    close(server);

    return 0;
}
