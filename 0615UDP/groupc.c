/*************************************************************************
	> File Name: UDPs.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月15日 星期四 16时41分34秒
 ************************************************************************/

#include"../header.h"
//发送端
int main()
{
    //创建发送端对象
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd < 0)
        return -1;

    //初始化接受端地址
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("224.0.0.100");
    addr.sin_port = htons(2520);

    //由于UDP没有connect函数，write函数不能发地址信息，只能用send函数
    sendto(fd, "xcgj", 4, 0, (struct sockaddr*)&addr, sizeof(addr));

    //接收回信
    char buf[4096];
    memset(buf, 0, sizeof(buf));
    read(fd, buf, sizeof(buf));    
    printf("%s\n", buf);

    close(fd);
    return 0;
}
