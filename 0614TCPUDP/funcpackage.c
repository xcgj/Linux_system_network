/*************************************************************************
  > File Name: funcpackage.c
  > Author: C01
  > Mail: 1003192057@qq.com 
  > Created Time: 2017年06月14日 星期三 16时34分59秒
 ************************************************************************/

#include"funcpackage.h"

//创建服务器socket，直接创建到监听步骤
int myServer(uint16_t port, const char * ip)
{
    //总的socket对象
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    //初始化地址
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);

    //绑定
    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        perror("bind");
        close(fd);
        return -1;
    }

    //监听
    listen(fd, 100);

    //返回socket对象
    return fd;
}

//创建客户端socket
int myClientConnect(uint16_t port, const char * ip)
{
    //创建对象
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    //初始化服务器地址
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    //连接
    int ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        perror("connect");
        return -1;
    }

    return fd;
}

//accept创建新的连接fd
int myAccept(int serverfd, struct sockaddr * addr, socklen_t * addrlen)
{
    int socketfd = -1;
    while(1)
    {
        //参数二三是获得客户端的ip和端口
        socketfd = accept(serverfd, addr, addrlen);
        if(socketfd == -1)
        {
            if(errno == EINTR)
            {
                continue;//信号打断
            }
            perror("accept");
            break;
        }
        break;
    }
    return socketfd;
}

//read，规定从管道读取固定的字节数，和读几次无关
int myRead(int socketfd, char * buf, int buflen)
{
    int readlen = 0;
    while(readlen != buflen)
    {
        int everyread = read(socketfd, buf+readlen, buflen-readlen);
        if(everyread > 0)
            readlen += everyread;
        else if(everyread == 0)
            break;//对方关闭了socket
        else if(everyread < 0)
        {
            if(errno == EINTR)
                continue;//信号打断
            return everyread;//严重错误
        }
    }
    return readlen;//返回真实读到的字节数
}

//文件有多少读多少
int readAny(int socketfd, char * buf, int buflen)
{
    int readbytes = -1;
    while(1)
    {
        readbytes = read(socketfd, buf, buflen);
        if(readbytes == -1 && errno == EINTR) continue;
        else if(readbytes < 0) break;
        break;
    }
    return readbytes;
}

//write，规定向管道写入固定的字节数
int myWrite(int socketfd, char * buf, int len)
{
    int writelen = 0;
    while(writelen != len)
    {
        int writeret = write(socketfd, buf+writelen, len-writelen);
        if(writeret > 0)
            writelen += writeret;
        //if(writeret == 0) //会阻塞等待
        else if(writeret < 0)
        {
            if(errno == EINTR)
                continue;
            break;
        }
    }
    return writelen;
}

//传输数据头部的数据大小标志
int myWriteLen(int fd, uint32_t len)
{
    len = htonl(len);//转网络字节序，大端
    int ret = myWrite(fd, (char *)&len, sizeof(len));
    return ret;
}

//接收数据头部的大小
int myReadLen(int fd, uint32_t * len)
{
    int ret = myRead(fd, (char * )len, sizeof(len));
    *len = ntohl(*len);
    return ret;
}

