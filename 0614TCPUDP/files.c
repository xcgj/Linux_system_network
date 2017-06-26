
#include "../network.h"

int main()
{
    int server = myServer(9988, "127.0.0.1");
    if(server == -1)
    {
        printf("create server error\n");
        return -1;
    }

    while(1)
    {
        int fd = myAccept(server, NULL, NULL);
        if(fd < 0)
        {
            printf("accept error\n");
            return -1;
        }

        uint32_t len;
        myReadLen(fd, &len);

        char* path = alloca(len+1);
        path[len] = 0;
        myRead(fd, path, len);

        int filefd = open(path, O_WRONLY|O_TRUNC|O_CREAT, 0777);

        myReadLen(fd, &len); // 读取文件长度
        char buf[4096];
        while(len > 0)
        {
            int readlen = 4096;
            if(len < 4096) readlen = len;

            int ret = myRead(fd, buf, readlen);
            if(ret > 0)
            {
                write(filefd, buf, ret);
                len -= ret;
            }
            else // <= 0
            {
                break;
            }
        }

        close(filefd);
        close(fd);
    }

    close(server);
}
