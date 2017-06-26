/*************************************************************************
	> File Name: LocalSocket.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月21日 星期三 23时12分25秒
 ************************************************************************/

#include"../header.h"
//服务器
int main()
{
    //创建对象
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);

    //初始化地址
    struct sockaddr_un addr;    //in:internet   un:unix
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "socket_file");//数组，在当前目录下创建socket文件

    //绑定地址
    bind(sock, (struct sockaddr*)&addr, sizeof(addr));

    //监听
    listen(sock, 20);

    //accept
    int newfd = accept(sock, NULL, NULL);

    char buf[1024];
    read(newfd, buf, sizeof(buf));

    printf("%s\n", buf);

    close(newfd);
    close(sock);
    return 0;
}
