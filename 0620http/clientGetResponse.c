//客户端向服务器发送的请求报文：
//  GET /xcgj.html HTTP/1.1
//  Host: 192.168.2.117:80
//  User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:35.0) Gecko/20100101 Firefox/35.0
//  Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
//  Accept-Language: en-US,en;q=0.5
//  Accept-Encoding: gzip, deflate
//  Connection: keep-alive

/* 业务：
 * 通过以上获取的请求报文
 * 自己按照格式向apache服务器写一个报文，并发送
 * 接收服务器返回的响应报文
 */ 

#include"../header.h"
//#include "../network.h"
//连接apache服务器的客户端
int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);//外网端口
    addr.sin_addr.s_addr = inet_addr("192.168.2.117");

    int ret = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    
    //发送请求给apache
    char buf[] = 
    "GET /xcgj.html HTTP/1.1\r\n"
    "Host: 192.168.2.117:80\r\n"
    "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:35.0) Gecko/20100101 Firefox/35.0\r\n"
    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
    "Accept-Language: en-US,en;q=0.5\r\n"
    "Accept-Encoding: gzip, deflate\r\n"
    "Connection: keep-alive\r\n"
    "\r\n";
    
    int len = strlen(buf);
    int writelen = 0;
    while(writelen != len)
    {
        int ret = write(sock, buf+writelen, len-writelen);
        if(ret > 0)
            writelen += ret;
        else if(ret <= 0)
        {
            if(errno == EINTR)
                continue;
            break;
        }
    }
    //myWrite(sock, buf, strlen(buf));

    //apache返回数据
    char recvbuf[8192];
    recv(sock, recvbuf, sizeof(recvbuf), 0);

    printf("%s\n", recvbuf);

    close(sock);
    return 0;
}
