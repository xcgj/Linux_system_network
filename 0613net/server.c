/*************************************************************************
	> File Name: server.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月13日 星期二 19时31分40秒
 ************************************************************************/

#include"../header.h"
// 服务器
int main()
{
    //1 创建socket对象
    //      返回值：文件描述符，用来创建数据通信的fd
    //      参数AF_INET：表示这个socket是用于internet网络通信的
    //      参数SOCK_STREAM：表示创建流套接字(TCP)，
    //          SOCK_DGRAM：创建UDP套接字
    //      参数三：表示协议，0表示默认协议，以太网就是IP协议
    int createfd = socket(AF_INET, SOCK_STREAM, 0);


    //2 指定地址：ip地址+端口号
    struct sockaddr_in addr;
    //      表示这个地址用于INET类型网络数据通信地址，要与定义的createfd的属性一致
    addr.sin_family = AF_INET;
    //      指示ip地址
    //addr.sin_addr.s_addr = inet_addr("192.168.87.128");
    addr.sin_addr.s_addr = 0;//表示要监听本机上的任何网口，0 == INADDR_ANY
    //addr.sin_addr.s_addr = inet_addr("127.0.0.1");//127.0.0.1是一个特殊的ip地址，表示本机
    //      指示端口，大端 host to netework - short
    addr.sin_port = htons(20170);

    
    //3 将地址和createfd关联,使用绑定函数
    //      地址需要强转 struct sockaddr*
    int ret = bind(createfd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret != 0)
    {
        //端口冲突
        perror("bind");
        return 0;
    }

    
    //4 监听
    //      使服务器处于被动等待连接状态
    //      参数二表示能够同时接纳的连接数，其实是一个缓冲区长度，队列长度
    //      监听步骤完成以后，才能获取客户端的连接请求
    listen(createfd, 250);


    //5 检查连接
    //      检查是否有人来连接我，这个调用可能是阻塞的，如果没人来连接，会阻塞等待
    //      返回值是一个新的socketfd，这个socketfd才是用于数据通信的
    //      createfd是用来接收连接，不停调用accept函数创建新的数据通信fd
    int serverfd = accept(createfd, NULL, NULL);
    if(serverfd < 0)
    {
        //中断打断或其他进程关闭了createfd
        perror("accept");
        return 0;
    }


    //6 从serverfd读取数据，也是阻塞的
    char buf[4096] = {0};
    memset(buf, 0, sizeof(buf));
    //      read返回实际的读取的数量，
    //      0：对方关闭了socket
    //      -1：读取失败
    read(serverfd, buf, sizeof(buf));
    printf("%s\n", buf);


    close(serverfd);
    close(createfd);
    return 0;
}



