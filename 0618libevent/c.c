/*************************************************************************
	> File Name: c.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月18日 星期日 17时43分27秒
 ************************************************************************/

#include"../header.h"

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("%d\n", fd);

    struct sockaddr_in addr;
    printf("con\n");
    addr.sin_family = AF_INET;
    printf("con\n");
    addr.sin_addr.s_addr = INADDR_ANY;
    printf("con\n");
    addr.sin_port = htons(9988);

    printf("con\n");
    int ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    printf("%d\n", ret);

    write(fd, "xcgj", 4);
    write(fd, "rbmw", 4);

    close(fd);
    return 0;
}
