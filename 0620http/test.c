/*************************************************************************
  > File Name: test.c
  > Author: C01
  > Mail: 1003192057@qq.com 
  > Created Time: 2017年06月20日 星期二 16时08分44秒
 ************************************************************************/

#include "../header.h"

int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9992);
    addr.sin_addr.s_addr = 0;

    int ret = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
        perror("bind"),exit(0);

    listen(sock, 10);

    int newfd = accept(sock, NULL, NULL);

    while(1)
    {
        char buf[4096];
        int ret = recv(newfd, buf, sizeof(buf), 0);
        if(ret > 0)
            printf("%s", buf);
        else
            break;
    }

    printf("\n");
}
