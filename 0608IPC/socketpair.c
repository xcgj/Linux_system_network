/*************************************************************************
	> File Name: socketpair.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月08日 星期四 17时48分58秒
 ************************************************************************/

#include"../header.h"
#include <sys/socket.h>

int main()
{
    int fd[2];
    socketpair(AF_UNIX, SOCK_STREAM, 0, fd);

    //写数据
    write(fd[0], "xcgj", 4);

    //读数据
    char buf[1024];
    int ret = read(fd[1], buf, sizeof(buf));
    printf("%s\n", buf);

    return 0;
}
