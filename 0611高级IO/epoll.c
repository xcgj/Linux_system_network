/*************************************************************************
	> File Name: epoll.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月11日 星期日 19时29分49秒
 ************************************************************************/

#include"../header.h"
#include<sys/epoll.h>

int main()
{
    //创建epoll的函数返回一个文件描述符
    int epollfd = epoll_create(1024);
    //创建产生事件的文件描述符
    int fdkey = open("/dev/input/event1", O_RDONLY);
    int fdmice = open("/dev/input/mice", O_RDONLY);

    //增加监控键盘事件
    struct epoll_event event;
    event.events = EPOLLIN;//监控可读
    event.data.fd = fdkey;//保存文件描述符，监控对象
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fdkey, &event);

    //增加监控鼠标事件
    //event参数是ctl的传入变量，并不是传出变量，会拷贝一份数据
    //所以外部变量可以复用
    event.data.fd = fdmice;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fdmice, &event);

    //捕捉事件的发生
    //事件数组，可能发生几个事件，就有多少个元素
    //但是每次wait返回后数组长度前几个都是存储了有数据发生的文件描述符
    struct epoll_event evs[2];
    char buf[1024];
    while(1)//可以在while函数里面增加文件描述符
    {
        int ret = epoll_wait(epollfd, evs, 2, 5000);

        //错误判断
        if(ret == 0)continue;//超时
        if(ret < 0 && errno == EINTR)continue;//被信号打断
        if(ret < 0)break;//严重错误

        //ret>0.收到ret个信号了
        //遍历evs的前ret个元素，查找信号是哪些描述符发出的
        int i = 0;
        for(; i<ret; ++i)
        {
            //依次获得有信号的文件描述符文件
            int fd = evs[i].data.fd;
            //依次清空描述符的数据
            read(fd, buf, sizeof(buf));
            //查找发出信号的描述符
            if(fd == fdkey)
                printf("键盘\n");
            else if(fd == fdmice)
                printf("鼠标\n");
        }
    }

    close(fdmice);
    close(fdkey);
    close(epollfd);

    return 0;
}

