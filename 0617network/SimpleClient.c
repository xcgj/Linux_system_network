/*************************************************************************
	> File Name: SimpleClient.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月17日 星期六 16时41分45秒
 ************************************************************************/
//测试客户端
#include"../header.h"

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(10000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(fd, (struct sockaddr*)&addr, sizeof(addr));

    close(fd);
    return 0;
}
