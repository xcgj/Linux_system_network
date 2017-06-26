/*************************************************************************
	> File Name: sf.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月13日 星期二 22时02分37秒
 ************************************************************************/

#include"../header.h"

int main()
{
    //创建socket对象
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    //初始化地址和端口
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(20004);

    //绑定地址
    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret != 0)
    {
        perror("bind");
        return 0;
    }

    //监听
    listen(fd, 200);

    //接受建立
    int sfd = accept(fd, NULL, NULL);
    if(sfd < 0)
    {
        perror("accept");
        return 0;
    }

    printf("连接建立\n");

    //读取信息
    char buf1[4096];
    char buf2[4096] = {0};
    memset(buf1, 0, sizeof(buf1));
    ret = read(sfd, buf1, sizeof(buf1));
    //printf("%s\n", buf);
    write(sfd, "OK", 2);

    //创建文件
    sprintf(buf2, "%scpy.txt", buf1);
    int ffd = open(buf2, O_CREAT|O_RDWR, 0777);

    //写文件
    while(1)
    {
        memset(buf1, 0, sizeof(buf1));
        int readbytes = read(ffd, buf1, sizeof(buf1));
        if(readbytes == 0)
            break;
    }

    close(ffd);
    
    close(sfd);
    close(fd);
    return 0;
}
