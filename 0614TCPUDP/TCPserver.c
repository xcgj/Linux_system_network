/*************************************************************************
	> File Name: TCPserver.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月14日 星期三 14时38分20秒
 ************************************************************************/

#include"../header.h"

int main()
{
    signal(SIGPIPE, SIG_IGN);

    //先创建socket对象
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    //初始化地址
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(25501);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //绑定地址
    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        perror("bind");
        return -1;
    }

    //监听
    listen(fd, 100);
    printf("等待链接\n");

    //接收连接
    //服务器需要多个socket对应连接多个客户端。用while()组建
    while(1)
    {
        char buf[4096];
        memset(buf, 0, sizeof(buf));
        int serfd = accept(fd, NULL, NULL);//socket
        if(serfd < 0)
        {
            if(errno == EINTR) continue;//中断打断
            break;
        }

        //处理数据
        int filefd = open("a", O_WRONLY|O_CREAT|O_TRUNC, 0777);
        while(1)
        {
            int readbytes = read(serfd, buf, sizeof(buf));//读管道
            if(readbytes == 0)
            {
                printf("客户端关闭了socket\n");
                break;
            }
            if(readbytes == -1 && errno == EINTR) continue;//信号打断
            if(readbytes == -1)
            {
                perror("read");
                break;
            }

            write(filefd, buf, readbytes);//写文件，每写一次，文件游标后移readbytes
        }

        close(filefd);
        close(serfd);
    }

    close(fd);
    return 0;
}
