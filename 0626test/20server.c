#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT 8888

int IsMoslems(char * ptr)
{
    if(ptr == NULL)
    {
        return 0;
    }
    char * buf = ptr;
    int len = strlen(buf);
    if(len <= 0) return 0;

    char * min = buf;
    char * max = buf + len - 1;
    int flag = 1;
    while(min < max)
    {
        if(*min != *max)
        {
            flag = 0;
            break;
        }

        ++min;
        ++max;
    }

    return flag;
}

int main()
{
    int createfd = socket(AF_INET, SOCK_STREAM, 0);
    if(createfd < 0) return -1;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = 0;
    addr.sin_port = htons(PORT);

    int ret = bind(createfd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0) return -2;

    listen(createfd, 20);

    int serverfd = accept(createfd, NULL, NULL);
    if(serverfd < 0) return -3;

    char buf[4096] = {0};
    memset(buf, 0, sizeof(buf));
    read(serverfd, buf, sizeof(buf));

    if(IsMoslems(buf))
    {
        printf("Moslems\n");
    }

    close(serverfd);
    close(createfd);
    return 0;
}
