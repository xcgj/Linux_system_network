/*************************************************************************
	> File Name: serverGetRequest.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月20日 星期二 14时53分27秒
 ************************************************************************/

#include"../header.h"
/* 业务：
 * 创建一个服务器，打印报文格式
 * 从客户端（浏览器）访问本机ip和地址
 * 服务器将在收到浏览器请求的时候打印这个浏览器发过来的报文
 */
int main()
{
    //TCP服务器创建
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = 0;

    int ret = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0) return 0;

    listen(sock, 10);
    
    int newfd = accept(sock, NULL, NULL);

    while(1)//直到对方关闭socket才停止读取
    {
        //获取请求报文格式
        char buf[4096];
        ret = recv(newfd, buf, sizeof(buf), 0);
        if(ret > 0)
            printf("%s", buf);
        else
            break;
    }

    printf("\n");
    
    close(newfd);
    close(sock);
    return 0;
}
