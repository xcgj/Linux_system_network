/*************************************************************************
	> File Name: UDPc.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月15日 星期四 16时44分21秒
 ************************************************************************/

#include"../header.h"
//接收端
int main()
{
    //创建接收端对象
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd < 0)
        return -1;

    //初始化接收端地址，方便绑定
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(2520);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //接收端绑定
    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
        return -1;

    //可以recv recvfrom read
    char buf[4096];
    memset(buf, 0, sizeof(buf));
    //read(fd, buf, sizeof(buf));//不关心对方地址
    //初始化地址容器，接收发送端的地址信息
    struct sockaddr peeraddr;
    socklen_t addrlen = sizeof(peeraddr);
    recvfrom(fd, buf, sizeof(buf), 0, &peeraddr, &addrlen);
    printf("%s\n", buf);
    
    //回信
    strcpy(buf, "rbmw");
    sendto(fd, buf, sizeof(buf), 0, &peeraddr, addrlen);

    close(fd);
    return 0;
}
