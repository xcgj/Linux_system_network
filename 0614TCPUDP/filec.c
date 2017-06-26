#include "funcpackage.h"

int main(int argc, char** argv)
{
    signal(SIGPIPE, SIG_IGN);
    const char* src = argv[1];
    const char* dst = argv[2];
    const char* ip = argv[3];
    const char* port = argv[4];

    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if(myConnect(fd, atoi(port), ip) < 0)
    {
        printf("connect server error\n");
        return -1;
    }

    uint32_t len = strlen(dst);
    myWriteLen(fd, len);
    myWrite(fd, dst, strlen(dst));

    int filefd = open(src, O_RDONLY);
    struct stat stat_buf;
    stat(src, &stat_buf);
    len = stat_buf.st_size;
    myWriteLen(fd, len); // 发送文件长度

    // 发送文件内容
    char buf[4096];
    while(1)
    {
        int ret = read(filefd, buf, sizeof(buf));
        if(ret == 0) break;

        myWrite(fd, buf, ret);
    }
}



