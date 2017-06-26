/*************************************************************************
	> File Name: client.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月13日 星期二 19时32分45秒
 ************************************************************************/

#include"../header.h"
// 客户端
int main()
{
    signal(SIGPIPE, SIG_IGN);//忽略信号

    //1 创建客户端socket对象
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);

    //客户端不需要指定地址，也就是说不需要bind，他的端口是系统随机分配的


    //2 指定连接地址。初始化服务器的地址信息
    //      指定连接对方的ip和端口
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.87.128");
    addr.sin_port = htons(20170);


    //3 连接操作
    //      参数一：谁要连接
    //      参数二：和谁连接
    if(connect(clientfd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        perror("connect");
    }


    //4 socket传入数据
    //      write时，如果服务器关闭socket，会收到SIGPIPE（管道破裂），导致程序退出
    //      可以注册SIGPIPE信号函数避免程序退出
    //      返回值是实际写入的数据数量，可能没有将所有数据写入socket
    int ret = write(clientfd, "hello xcgj", 10);


    close(clientfd);
    return 0;
}
