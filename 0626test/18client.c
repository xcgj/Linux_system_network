
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define HOSTADDR "192.168.0.1"

int main()
{
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if(clientfd < 0) return -1;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = inet_addr(HOSTADDR);

    int connectret = connect(clientfd, (struct sockaddr*)&addr, sizeof(addr));
    if(connectret < 0) return -2;

    //...read&write

    close(clientfd);
    return 0;
}
