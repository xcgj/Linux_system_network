/*************************************************************************
	> File Name: LocalSocket.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月21日 星期三 23时12分25秒
 ************************************************************************/

#include"../header.h"
//客户端
int main()
{
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "socket_file");

    connect(sock, (struct sockaddr*)&addr, sizeof(addr));

    write(sock, "xcgj", 5);

    close(sock);
    return 0;
}
