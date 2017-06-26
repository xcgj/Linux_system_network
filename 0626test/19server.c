#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT 8888


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

    struct sockaddr_in addr_client;
    socklen_t len = sizeof(addr_client);
    int serverfd = accept(createfd, (struct sockaddr*)&addr_client, &len);
    if(serverfd < 0) return -3;

    char buf[4096] = {0};
    memset(buf, 0, sizeof(buf));
    read(serverfd, buf, sizeof(buf));
    int buflen = strlen(buf);

    sendto(serverfd, buf, buflen, 0, (struct sockaddr*)&addr_client, len);

    close(serverfd);
    close(createfd);
    return 0;
}
